/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <functional>
#include <memory>
#include <esignal/debug.h>
#include <esignal/Base.h>
#include <esignal/LockSharedPtrRef.h>
#include <esignal/RefCount.h>
#include <esignal/Connection.h>
#include <functional>
#include <type_traits>
#include <utility>
#include <mutex>

namespace esignal {
	
	#undef __class__
	#define __class__ "Signal<T>"
	template<class... Args>
	class Signal : public esignal::Base {
		public:
			using Observer = std::function<void(const Args&...)>;
			int32_t m_callInProgress;
		public:
			Signal() :
			  m_callInProgress(0) {
				
			}
			// copy constructor:
			Signal(const Signal&) = delete;
			// copy operator:
			Signal& operator=(Signal) = delete;
			Signal& operator=(const Signal& _obj) = delete;
			// Move constructor
			Signal(Signal&& _obj) = delete;
			// Move operator
			Signal& operator=(Signal&& _obj) = delete;
		private:
			class Executor {
				public:
					Observer m_observer;
					bool m_removed;
					size_t m_uid;
				public:
					Executor(Observer&& _observer):
					  m_removed(false),
					  m_uid(0) {
						m_uid = s_uid++;
						m_observer = std::move(_observer);
					}
					
					virtual ~Executor() = default;
				public:
					virtual void emit(Args... _values) {
						if (m_removed == true) {
							return;
						}
						try {
							m_observer(_values...);
						} catch(...) {
							m_removed = true;
							std::cout << "LOL"<< std::endl;
						}
					}
			};
		protected:
			std::vector<std::unique_ptr<Executor>> m_executors;
		private:
			class ExecutorShared : public Executor {
				protected:
					std::weak_ptr<void> m_object;
				public:
					ExecutorShared(std::weak_ptr<void> _object, Observer&& _observer) :
					  Executor(std::move(_observer)),
					  m_object(_object) {
						
					}
				public:
					virtual void emit(Args... _values) {
						std::shared_ptr<void> destObject = m_object.lock();
						if (destObject == nullptr) {
							Executor::m_removed = true;
							return;
						}
						if (Executor::m_removed == true) {
							return;
						}
						try {
							Executor::m_observer(_values...);
						} catch(...) {
							Executor::m_removed = true;
							std::cout << "LOL"<< std::endl;
						}
					}
			};
		public:
			template< class ObserverType >
			Connection connect(ObserverType&& observer ) {
				std::unique_ptr<Executor> executer(new Executor(std::forward<ObserverType>(observer)));
				std::size_t uid = executer->m_uid;
				m_executors.push_back(std::move(executer));
				return Connection(Base::m_shared, uid);
			}
			template<class classType, class Func, class... Arg>
			Connection connect(classType* _class, Func _f, Arg... _arg) {
				std::unique_ptr<Executor> executer(new Executor([=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
				std::size_t uid = executer->m_uid;
				m_executors.push_back(std::move(executer));
				return Connection(Base::m_shared, uid);
			}
			/*
			template<class classType, class Func, class... Arg>
			void connect(const std::shared_ptr<classType>& _class, Func _f, Arg... _arg) {
				classType* tmp = _class.get();
				std::unique_ptr<ExecutorShared> executer(new ExecutorShared(_class, [=]( auto&&... cargs ){
					(*tmp.*_f)(cargs..., _arg... );
				}));
				m_executors.push_back(std::move(executer));
			}
			*/
			template<class classType, class TYPE, typename... Arg>
			void connect(const std::shared_ptr<classType>& _class, void (TYPE::*_func)(const Args&..., Arg...), Arg... _args) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_class);
				if (obj2 == nullptr) {
					ESIGNAL_ERROR("Can not bind signal ...");
					return;
				}
				TYPE* directPointer = obj2.get();
				std::unique_ptr<ExecutorShared> executer(new ExecutorShared(_class, [=]( auto&&... cargs ){
					// TODO : Check if compilator does not use the shared ptr ...
					(*directPointer.*_func)(cargs..., _args... );
				}));
				m_executors.push_back(std::move(executer));
			}
			/*
			template<class pointer, class Func, class... Arg>
			void bind(const std::shared_ptr<pointer>& _class, Func _f, const Arg&... _arg) {
				std::unique_ptr<ExecutorShared> executer(new ExecutorShared(_class, [=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
				m_executors.push_back(std::move(executer));
			}
			*/
		public:
			template< class... CallArgs>
			void emit( CallArgs&&... args) {
				m_callInProgress++;
				for (size_t iii=0; iii < m_executors.size(); ++iii) {
					m_executors[iii]->emit(args...);
				}
				if (m_callInProgress == 1) {
					auto it = m_executors.begin();
					while (it != m_executors.end()) {
						if (    *it == nullptr
						     || (*it)->m_removed == true) {
							it = m_executors.erase(it);
							continue;
						}
						++it;
					}
				}
				m_callInProgress--;
			}
			void disconnect(std::size_t _uid) {
				for (size_t iii=0; iii < m_executors.size(); ++iii) {
					if (m_executors[iii]->m_uid == _uid) {
						m_executors[iii]->m_removed = true;
						return;
					}
				}
			}
		public:
			size_t size() const {
				return m_executors.size();
			}
			
			bool empty() const {
				return m_executors.empty();
			}
			
			void clear() {
				m_executors.clear();
			}
	};
	#undef __class__
	#define __class__ nullptr
}


