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
#include <functional>
#include <type_traits>
#include <utility>
#include <mutex>

namespace esignal {
	extern size_t s_uid;
	
	template<class TYPE>
	class RefCount {
		public:
			std::mutex m_lock;
			int64_t m_count;
			TYPE* m_data;
		public:
			RefCount(TYPE* _data) :
			  m_count(0),
			  m_data(_data) {
				// nothing to do.
			}
			// copy constructor:
			RefCount(const RefCount&) = delete;
			// copy operator:
			RefCount& operator=(RefCount) = delete;
			RefCount& operator=(const RefCount& _obj) = delete;
			// Move constructor
			RefCount(RefCount&& _obj) = delete;
			// Move operator
			RefCount& operator=(RefCount&& _obj) = delete;
		public:
			~RefCount() {
				m_data = nullptr;
			}
		public:
			void lock() {
				m_lock.lock();
			}
			void unlock() {
				m_lock.unlock();
			}
			void inc() {
				lock();
				m_count++;
				unlock();
			}
			int64_t dec() {
				int64_t val;
				lock();
				m_count--;
				val = m_count;
				unlock();
				return val;
			}
			int64_t getCount() const {
				return m_count;
			}
			void remove() {
				lock();
				m_data = nullptr;
				unlock();
			}
			TYPE* get() {
				return m_data;
			}
	};
	template<class TYPE>
	class lockSharedPtrRef {
		public:
			RefCount<TYPE>* m_counter;
		public:
			lockSharedPtrRef(TYPE* _pointer) :
			  m_counter(nullptr) {
				if (_pointer != nullptr) {
					m_counter = new RefCount<TYPE>(_pointer);
					m_counter->inc();
				}
			}
			// copy constructor:
			lockSharedPtrRef(const lockSharedPtrRef& _obj) :
			  m_counter(_obj.m_counter) {
				if (m_counter == nullptr) {
					return;
				}
				m_counter->inc();
			}
			// copy operator:
			lockSharedPtrRef& operator=(lockSharedPtrRef) = delete;
			lockSharedPtrRef& operator=(const lockSharedPtrRef& _obj) {
				if (&_obj == this) {
					return *this;
				}
				if (m_counter != nullptr) {
					m_counter->dec();
					m_counter = nullptr;
				}
				m_counter = _obj.m_counter;
				if (m_counter == nullptr) {
					return;
				}
				m_counter->inc();
				return *this;
			}
			// Move constructor
			lockSharedPtrRef(lockSharedPtrRef&& _obj) :
			  m_counter(std::move(_obj.m_counter)) {
				
			}
			// Move operator
			lockSharedPtrRef& operator=(lockSharedPtrRef&& _obj) {
				m_counter = std::move(_obj.m_counter);
			}
			~lockSharedPtrRef() {
				int64_t count = m_counter->dec();
				if (count > 0) {
					return;
				}
				delete m_counter;
				m_counter = nullptr;
			}
			void removeData() {
				if (m_counter != nullptr) {
					m_counter->remove();
				}
			}
			void disconnect(std::size_t _uid) {
				if (m_counter == nullptr) {
					return;
				}
				m_counter->lock();
				TYPE* val = m_counter->get();
				if (val != nullptr) {
					val->disconnect(_uid);
				}
				m_counter->unlock();
			}
	};
	
	#undef __class__
	#define __class__ "Signal<T>"
	class SignalInderection;
	class SignalBase {
		protected:
			lockSharedPtrRef<SignalBase> m_shared;
		public:
			SignalBase() :
			  m_shared(this) {
				
			}
			// copy constructor:
			SignalBase(const SignalBase&) = delete;
			// copy operator:
			//SignalBase& operator=(SignalBase) = delete;
			//SignalBase& operator=(const SignalBase& _obj) = delete;
			// Move constructor
			SignalBase(SignalBase&& _obj) = delete;
			// Move operator
			//SignalBase& operator=(SignalBase&& _obj) = delete;
			
			virtual ~SignalBase() {
				m_shared.removeData();
			}
			virtual void disconnect(std::size_t _uid) = 0;
	};
	
	
	class Connection {
		public:
			Connection(lockSharedPtrRef<SignalBase> _ref, std::size_t _id):
			  m_signalRefUnique(_ref), m_uid(_id) {
				
			}
			Connection(const Connection&) = delete; // not copyable
			Connection* operator=(const Connection&) = delete; // no copy operator
			Connection(Connection&&) = default; // movable
			Connection& operator=(Connection&&) = default; // movable op
			
			~Connection() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
			void disconnect() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
		private:
			lockSharedPtrRef<SignalBase> m_signalRefUnique;
			std::size_t m_uid;
	};
	
	template<class... Args>
	class Signal : public SignalBase {
		public:
			using Observer = std::function<void(const Args&...)>;
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
				return Connection(SignalBase::m_shared, uid);
			}
			template<class pointer, class Func, class... Arg>
			Connection connect(pointer* _class, Func _f, Arg... _arg) {
				std::unique_ptr<Executor> executer(new Executor([=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
				std::size_t uid = executer->m_uid;
				m_executors.push_back(std::move(executer));
				return Connection(SignalBase::m_shared, uid);
			}
			template<class pointer, class Func, class... Arg>
			void connect(const std::shared_ptr<pointer>& _class, Func _f, Arg... _arg) {
				std::unique_ptr<ExecutorShared> executer(new ExecutorShared(_class, [=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
				m_executors.push_back(std::move(executer));
			}
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
			size_t size() const {
				return m_executors.size();
			}
			
			bool empty() const {
				return m_executors.empty();
			}
			
			void clear() {
				m_executors.clear();
			}
		private:
			std::vector<std::unique_ptr<Executor>> m_executors;
			int32_t m_callInProgress;
	};
	#undef __class__
	#define __class__ nullptr
}


