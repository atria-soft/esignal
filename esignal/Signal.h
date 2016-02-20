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
	/**
	 * @brief Basic signal base
	 * @param[in] Args... Argument of the signal
	 */
	template<class... Args>
	class Signal : public esignal::Base {
		public:
			using Observer = std::function<void(const Args&...)>; //!< Define an Observer: function pointer
		protected:
			int32_t m_callInProgress; //!< know if we are in a recursive loop
		public:
			//! @brief Basic constructor
			Signal():
			  m_callInProgress(0) {
				
			}
			//! @brief Copy constructor (REMOVED)
			Signal(const Signal&) = delete;
			//! @brief Copy operator (REMOVED)
			Signal& operator=(Signal) = delete;
			Signal& operator=(const Signal& _obj) = delete;
			//! @brief Move constructor (REMOVED)
			Signal(Signal&& _obj) = delete;
			//! @brief Move operator
			Signal& operator=(Signal&& _obj) = delete;
		private:
			/**
			 * @brief Executor: Class to manage the UID and basic value of an observer
			 */
			class Executor {
				public:
					Observer m_observer; //!< Observer to call when needed (if not removed).
					bool m_removed; //!< the executor has been removed.
					size_t m_uid; //!< unique ID of the signal (used to remove it).
				public:
					/**
					 * @brief Basic constructor.
					 * @param[in] _observer Observer to call.
					 */
					Executor(Observer&& _observer):
					  m_removed(false),
					  m_uid(0) {
						m_uid = s_uid++;
						m_observer = std::move(_observer);
					}
					//! @brief virtual destructor.
					virtual ~Executor() = default;
				public:
					/**
					 * @brief Emit the data on the observer.
					 * @param[in] _values... Multiple value needed to send on observers
					 */
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
			std::vector<std::unique_ptr<Executor>> m_executors; //!< List of all executors.
		private:
			/**
			 * @brief Executor specific to the Shared_ptr caller that does not want to worry about the removing of the signal.
			 * @param[in] Args... Argument of the signal
			 */
			class ExecutorShared : public Executor {
				protected:
					std::weak_ptr<void> m_object; //!< a weak reference on the object to verify that it is alive
				public:
					/**
					 * @brief shared constructor.
					 * @param[in] _object A weak reference of the object.
					 * @param[in] _observer Observer to call.
					 */
					ExecutorShared(std::weak_ptr<void> _object, Observer&& _observer) :
					  Executor(std::move(_observer)),
					  m_object(_object) {
						
					}
				public:
					/**
					 * @brief Emit the data on the observer.
					 * @param[in] _values... Multiple value needed to send on observers
					 */
					virtual void emit(Args... _values) {
						// TODO: maybe an error if the object is not manage by the same thread.
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
			/**
			 * @brief Connect an observer on the signal.
			 * @param[in] _observer Observer to call.
			 */
			template< class ObserverType >
			Connection connect(ObserverType&& _observer ) {
				std::unique_ptr<Executor> executer(new Executor(std::forward<ObserverType>(_observer)));
				std::size_t uid = executer->m_uid;
				m_executors.push_back(std::move(executer));
				return Connection(Base::m_shared, uid);
			}
			/**
			 * @brief Connect an function member on the signal.
			 * @param[in] _class Object on whe we need to call.
			 * @param[in] _func Function to call.
			 * @param[in] _arg Argument optinnal the user want to add.
			 */
			template<class classType, class Func, class... Arg>
			Connection connect(classType* _class, Func _func, Arg... _arg) {
				std::unique_ptr<Executor> executer(new Executor([=]( auto&&... cargs ){
					(*_class.*_func)(cargs..., _arg... );
				}));
				std::size_t uid = executer->m_uid;
				m_executors.push_back(std::move(executer));
				return Connection(Base::m_shared, uid);
			}
			/**
			 * @brief Connect an function member on the signal with the shared_ptr object.
			 * @param[in] _class shared_ptr Object on whe we need to call ==> the object is get in keeped in weak_ptr.
			 * @param[in] _func Function to call.
			 * @param[in] _arg Argument optinnal the user want to add.
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
		public:
			/**
			 * @brief Emit data on the signal.
			 * @param[in] _args Argument data to emit.
			 */
			template< class... CallArgs>
			void emit(CallArgs&&... _args) {
				// TODO : Add protection ... but how ...
				m_callInProgress++;
				for (size_t iii=0; iii < m_executors.size(); ++iii) {
					m_executors[iii]->emit(_args...);
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
			/**
			 * @brief Disconnect an observer of the signal.
			 * @param[in] _uid Unique id of the signal.
			 */
			void disconnect(std::size_t _uid) {
				for (size_t iii=0; iii < m_executors.size(); ++iii) {
					if (m_executors[iii]->m_uid == _uid) {
						m_executors[iii]->m_removed = true;
						return;
					}
				}
			}
		public:
			/**
			 * @brief Get the number of observers connected on the signal.
			 * @return The count of observer.
			 */
			size_t size() const {
				return m_executors.size();
			}
			/**
			 * @brief Check if we have a connected observers.
			 * @return true More than one observers.
			 * @return false No observers.
			 */
			bool empty() const {
				return m_executors.empty();
			}
			/**
			 * @brief Clear all connectd observers.
			 */
			void clear() {
				m_executors.clear();
			}
	};
}


