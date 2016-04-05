/** @file
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
	template<class... T_ARGS>
	class Signal : public esignal::Base {
		public:
			using Observer = std::function<void(const T_ARGS&...)>; //!< Define an Observer: function pointer
		protected:
			int32_t m_callInProgress; //!< know if we are in a recursive loop
		public:
			/**
			 * @brief Basic constructor with connection observer
			 * @param[in] _countObs Local observer to know the count of connection on the signal.
			 */
			Signal(ObserverConnection _countObs=nullptr);
			/**
			 * @brief Basic constructor with connection observer
			 * @param[in] _class Class which is associated the function to call.
			 * @param[in] _func Function to call when an observer has connected or disconnected.
			 */
			template<class CLASS_TYPE, class FUNC_TYPE>
			Signal(CLASS_TYPE* _class, FUNC_TYPE _func);
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
					Executor(Observer&& _observer);
					//! @brief virtual destructor.
					virtual ~Executor() = default;
				public:
					/**
					 * @brief Emit the data on the observer.
					 * @param[in] _values... Multiple value needed to send on observers
					 */
					virtual void emit(const T_ARGS&... _values);
				public:
					/**
					 * @brief check if the Executor is a managed by this shared_ptr
					 * @param[in] _obj Object to check
					 * @return true The Executor depend on this shared_ptr
					 * @return false The Executor does not depend on this shared_ptr
					 */
					virtual bool isSharedPtr(const std::shared_ptr<void>& _obj);
			};
		protected:
			std::vector<std::unique_ptr<Executor>> m_executors; //!< List of all executors.
		private:
			/**
			 * @brief Executor specific to the Shared_ptr caller that does not want to worry about the removing of the signal.
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
					ExecutorShared(std::weak_ptr<void> _object, Observer&& _observer);
				public:
					/**
					 * @brief Emit the data on the observer.
					 * @param[in] _values... Multiple value needed to send on observers
					 */
					virtual void emit(const T_ARGS&... _values);
				public:
					bool isSharedPtr(const std::shared_ptr<void>& _obj) override;
			};
		public:
			/**
			 * @brief Connect an observer on the signal.
			 * @param[in] _observer Observer to call.
			 * @return Connection handle (connection is removed if the handle is destroy)
			 */
			template< class OBSERVER_TYPE >
			esignal::Connection connect(OBSERVER_TYPE&& _observer);
			/**
			 * @brief Connect an function member on the signal.
			 * @param[in] _class Object on whe we need to call.
			 * @param[in] _func Function to call.
			 * @param[in] _arg Argument optinnal the user want to add.
			 * @return Connection handle (connection is removed if the handle is destroy)
			 */
			template<class CLASS_TYPE, class FUNC_TYPE, class... FUNC_ARGS_TYPE>
			esignal::Connection connect(CLASS_TYPE* _class,
			                            FUNC_TYPE _func,
			                            FUNC_ARGS_TYPE... _arg);
			/**
			 * @brief Connect an function member on the signal with the shared_ptr object.
			 * @param[in] _class shared_ptr Object on whe we need to call ==> the object is get in keeped in weak_ptr.
			 * @param[in] _func Function to call.
			 * @param[in] _args Argument optinnal the user want to add.
			 */
			template<class PARENT_CLASS_TYPE, class CLASS_TYPE, typename... FUNC_ARGS_TYPE>
			void connect(const std::shared_ptr<PARENT_CLASS_TYPE>& _class,
			             void (CLASS_TYPE::*_func)(const T_ARGS&..., FUNC_ARGS_TYPE...),
			             FUNC_ARGS_TYPE... _args);
		public:
			/**
			 * @brief Emit data on the signal.
			 * @param[in] _args Argument data to emit.
			 */
			void emit(const T_ARGS&... _args);
		protected:
			/**
			 * @brief Remove observer in the internal list if the user has disconnected
			 * @note Done only if it is the last caller ...
			 */
			void removeIfPossible();
		public:
			/**
			 * @brief Disconnect an observer of the signal.
			 * @param[in] _uid Unique id of the signal connection.
			 */
			void disconnect(size_t _uid) override;
			/**
			 * @brief Disconnect the shared_ptr form the Signal
			 * @param[in] _obj Link with the object to check
			 */
			void disconnectShared(const std::shared_ptr<void>& _obj) override;
			/**
			 * @brief Get the number of observers connected on the signal.
			 * @return The count of observer.
			 */
			size_t size() const;
			/**
			 * @brief Check if we have a connected observers.
			 * @return true More than one observers.
			 * @return false No observers.
			 */
			bool empty() const;
			/**
			 * @brief Clear all connectd observers.
			 */
			void clear();
	};
}


template<class... T_ARGS>
template< class OBSERVER_TYPE >
esignal::Connection esignal::Signal<T_ARGS...>::connect(OBSERVER_TYPE&& _observer ) {
	ESIGNAL_DEBUG("esignal: '" << getName() << "' try connect: '" << getName() << "' (observer)");
	std::unique_ptr<Executor> executer(new Executor(std::forward<OBSERVER_TYPE>(_observer)));
	size_t uid = executer->m_uid;
	m_executors.push_back(std::move(executer));
	if (m_connectionObserver!=nullptr) {
		m_connectionObserver(m_executors.size());
	}
	ESIGNAL_DEBUG("     '" << getName() << "' new count: " << m_executors.size());
	return esignal::Connection(Base::m_shared, uid);
}

template<class... T_ARGS>
template<class CLASS_TYPE, class FUNC_TYPE, class... FUNC_ARGS_TYPE>
esignal::Connection esignal::Signal<T_ARGS...>::connect(CLASS_TYPE* _class,
                                                        FUNC_TYPE _func,
                                                        FUNC_ARGS_TYPE... _arg) {
	ESIGNAL_DEBUG("esignal: '" << getName() << "' try connect: '" << getName() << "' (reference)");
	if (_class == nullptr) {
		ESIGNAL_ERROR("     '" << getName() << "' Class pointer in nullptr");
		return esignal::Connection();
	}
	std::unique_ptr<Executor> executer(new Executor([=](const T_ARGS& ... _argBase){
		(*_class.*_func)(_argBase..., _arg... );
	}));
	size_t uid = executer->m_uid;
	m_executors.push_back(std::move(executer));
	if (m_connectionObserver != nullptr) {
		m_connectionObserver(m_executors.size());
	}
	ESIGNAL_DEBUG("     '" << getName() << "' new count: " << m_executors.size());
	return Connection(Base::m_shared, uid);
}

template<class... T_ARGS>
template<class PARENT_CLASS_TYPE, class CLASS_TYPE, typename... FUNC_ARGS_TYPE>
void esignal::Signal<T_ARGS...>::connect(const std::shared_ptr<PARENT_CLASS_TYPE>& _class,
                                         void (CLASS_TYPE::*_func)(const T_ARGS&..., FUNC_ARGS_TYPE...),
                                         FUNC_ARGS_TYPE... _args) {
	ESIGNAL_DEBUG("esignal: '" << getName() << "' try connect: '" << getName() << "' (weak pointer)");
	if (_class == nullptr) {
		ESIGNAL_ERROR("     '" << getName() << "' Class pointer in nullptr");
		return;
	}
	std::shared_ptr<CLASS_TYPE> obj2 = std::dynamic_pointer_cast<CLASS_TYPE>(_class);
	if (obj2 == nullptr) {
		ESIGNAL_ERROR("Can not connect signal ...");
		return;
	}
	CLASS_TYPE* directPointer = obj2.get();
	std::unique_ptr<ExecutorShared> executer(new ExecutorShared(_class, [=]( const T_ARGS& ... _argBase){
		// TODO : Check if compilator does not use the shared ptr ...
		(*directPointer.*_func)(_argBase..., _args... );
	}));
	m_executors.push_back(std::move(executer));
	if (m_connectionObserver!=nullptr) {
		m_connectionObserver(m_executors.size());
	}
	ESIGNAL_DEBUG("     '" << getName() << "' new count: " << m_executors.size());
}

template<class... T_ARGS>
template<class CLASS_TYPE, class FUNC_TYPE>
esignal::Signal<T_ARGS...>::Signal(CLASS_TYPE* _class,
                                   FUNC_TYPE _func):
  esignal::Base([=](size_t _val){(*_class.*_func)(_val);}),
  m_callInProgress(0) {
	// nothing to do
}
