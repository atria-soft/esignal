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

namespace esignal {
	#undef __class__
	#define __class__ "Signal<T>"
	template<typename T> class Signal : public esignal::Base {
		private:
			std::vector<std::pair<std::weak_ptr<void>,
			                      std::function<void(const T&)>>> m_callerList; // current list of binded element
			std::vector<std::pair<std::weak_ptr<void>,
			                      std::function<void(const T&)>>> m_callerListInCallback; // temporaty list (when add one in call process)
			std::vector<std::function<void(const T&)>> m_callerListDirect; // current list of binded element
			std::vector<std::function<void(const T&)>> m_callerListDirectInCallback; // temporaty list (when add one in call process)
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
			 */
			Signal(esignal::Interface& _signalInterfaceLink,
			       const std::string& _name,
			       const std::string& _description = "",
			       bool _periodic = false);
			/**
			 * @brief Destructor.
			 */
			virtual ~Signal();
			/**
			 * @brief Bind a callback function to the current signal (generic methis (simplest))
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func Link on the fuction that might be called (inside a class)
			 * @example signalXXXX.bind(shared_from_this(), &ClassName::onCallbackXXX);
			 */
			template<class TYPE_CLASS, class TYPE, typename... TArgs>
			void bind(std::shared_ptr<TYPE_CLASS> _obj, void (TYPE::*_func)(const T&, TArgs...), TArgs... _args2) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					ESIGNAL_ERROR("Can not bind signal ...");
					return;
				}
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1, std::forward<TArgs>(_args2)...)));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1, std::forward<TArgs>(_args2)...)));
				}
			}
			/**
			 * @brief Advanced binding a callback function to the current signal.
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func functor to call (do it yourself)
			 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
			 */
			void connect(std::shared_ptr<void> _obj, std::function<void(const T&)> _function );
			//! @previous
			void connect(std::function<void(const T&)> _function );
			/**
			 * @brief Check if an object is registered in the Signal
			 * @param[in] _obj shared pointer on the object
			 * @return true The object is connected at this signal.
			 * @return false The object is NOT connected on this signal.
			 */
			bool isRegistered(std::shared_ptr<void> _obj);
			/**
			 * @brief remove link on the signal.
			 * @param[in] _obj shared pointer on the removing object
			 */
			void release(std::shared_ptr<void> _obj);
			/**
			 * @brief Generate a signal on all interface listening.
			 * @param[in] _data data to emit
			 */
			void emit(const T& _data);
			size_t getNumberConnected();
	};
	#undef __class__
	#define __class__ "Signal<void>"
	template<> class Signal<void> : public esignal::Base {
		private:
			std::vector<std::pair<std::weak_ptr<void>, std::function<void()>>> m_callerList;
			std::vector<std::pair<std::weak_ptr<void>, std::function<void()>>> m_callerListInCallback;
			std::vector<std::function<void()>> m_callerListDirect;
			std::vector<std::function<void()>> m_callerListDirectInCallback;
		public:
			/**
			 * @brief Create a signal with a specific 'void' type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
			 */
			Signal(esignal::Interface& _signalInterfaceLink,
			      const std::string& _name,
			      const std::string& _description = "",
			      bool _periodic = false);
			/**
			 * @brief Destructor.
			 */
			virtual ~Signal();
			/**
			 * @brief Bind a callback function to the current signal (generic methis (simplest))
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func Link on the fuction that might be called (inside a class)
			 * @example signalXXXX.connect(shared_from_this(), &ClassName::onCallbackXXX);
			 */
			template<class TYPE_CLASS, class TYPE, typename... TArgs>
			void bind(std::shared_ptr<TYPE_CLASS> _obj, void (TYPE::*_func)(TArgs...), TArgs... args2) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					ESIGNAL_ERROR("Can not bind signal ...");
					return;
				}
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::forward<TArgs>(args2)...)));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::forward<TArgs>(args2)...)));
				}
			}
			/**
			 * @brief Advanced binding a callback function to the current signal.
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func functor to call (do it yourself)
			 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
			 */
			void connect(std::shared_ptr<void> _obj, std::function<void()> _function);
			//! @previous
			void connect(std::function<void()> _function);
			/**
			 * @brief remove link on the signal.
			 * @param[in] _obj shared pointer on the removing object
			 */
			void release(std::shared_ptr<void> _obj);
			void emit();
			size_t getNumberConnected();
	};
	#undef __class__
	#define __class__ nullptr
}

