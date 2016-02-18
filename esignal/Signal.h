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

namespace esignal {
	#undef __class__
	#define __class__ "Signal<T>"
	template<class... Args>
	class Signal {
		public:
			using Observer = std::function<void(const Args&...)>;
		private:
			
			class Executor {
				protected:
					Observer m_observer;
				public:
					Executor(Observer&& _observer) {
						m_observer = std::move(_observer);
					}
					virtual ~Executor() = default;
				public:
					virtual bool emit(Args... _values) {
						try{
							m_observer( _values... );
						} catch(...) {
							std::cout << "LOL"<< std::endl;
						}
						return false;
					}
			};
			
			class ExecutorShared : public Executor {
				protected:
					std::week_ptr<void> m_object;
				public:
					ExecutorShared(std::weak_ptr<void> _object, Observer&& _observer) :
					  Executor(std::move(_observer)),
					  m_object(_object) {
						
					}
				public:
					virtual bool emit(Args... _values) {
						try{
							m_observer( _values... );
						} catch(...) {
							std::cout << "LOL"<< std::endl;
						}
						return false;
					}
			};
		public:
			
			template< class ObserverType >
			void connect( ObserverType&& observer ) {
				m_observers.push_back( std::forward<ObserverType>(observer) );
				m_executors.push_back(Executor(std::forward<ObserverType>(observer)));
			}
			template< class... CallArgs>
			void emit( CallArgs&&... args ) {
				for(auto& executor: m_executors) {
					executor.emit(args... );
				}
			}
			template<class pointer, class Func, class... Arg>
			void connect2(pointer* _class, Func _f, Arg... _arg) {
				m_observers.push_back( [=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				});
				m_executors.push_back(Executor([=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
			}
			template<class pointer, class Func, class... Arg>
			void connect2(std::shared_ptr<pointer> _class, Func _f, Arg... _arg) {
				m_executors.push_back(Executor([=]( auto&&... cargs ){
					(*_class.*_f)(cargs..., _arg... );
				}));
			}

		private:
			std::vector<Observer> m_observers;
			std::vector<Executor> m_executors;
	};
	
	
	
	#if 0
	template<typename... T> class Signal {
		public:
			using Observer std::function<void(const T&...)>;
		private:
			std::vector<std::pair<std::weak_ptr<void>, Observer>> m_callerList; // current list of binded element
			std::vector<std::pair<std::weak_ptr<void>, Observer>> m_callerListInCallback; // temporaty list (when add one in call process)
			std::vector<Observer> m_callerListDirect; // current list of binded element
			std::vector<Observer> m_callerListDirectInCallback; // temporaty list (when add one in call process)
			int32_t m_callInProgress;
			bool m_someOneRemoveInCall;
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
			 */
			Signal() :
			  m_callInProgress(0),
			  m_someOneRemoveInCall(false) {
				
			}
			
			/**
			 * @brief Destructor.
			 */
			~Signal() = default;
			/**
			 * @brief Bind a callback function to the current signal (generic methis (simplest))
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func Link on the fuction that might be called (inside a class)
			 * @example signalXXXX.bind(shared_from_this(), &ClassName::onCallbackXXX);
			 */
			template<class TYPE_CLASS, class TYPE, typename... TArgs>
			void bind(std::shared_ptr<TYPE_CLASS> _obj, void (TYPE::*_func)(const T&..., TArgs...), TArgs... _args2) {
				std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
				if (obj2 == nullptr) {
					ESIGNAL_ERROR("Can not bind signal ...");
					return;
				}
				if (m_callInProgress == 0) {
					if (COUNT_ELEM == 1) {
						m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), [](TArgs...){obj2.get()->, std::placeholders::_1, std::forward<TArgs>(_args2)...)));
					}
				} else {
					// m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), std::bind(_func, obj2.get(), std::placeholders::_1, std::forward<TArgs>(_args2)...)));
				}
			}
			/**
			 * @brief Advanced binding a callback function to the current signal.
			 * @param[in] _obj Shared pointer on the caller object
			 * @param[in] _func functor to call (do it yourself)
			 * @example signalXXXX.connect(shared_from_this(), std::bind(&ClassName::onCallbackXXX, this, std::placeholders::_1));
			 */
			void connect(std::shared_ptr<void> _obj, std::function<void(const T&...)> _function ) {
				if (m_callInProgress == 0) {
					m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				} else {
					m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
				}
			}
						//! @previous
						void connect(std::function<void(const T&...)> _function );{
				if (m_callInProgress == 0) {
					m_callerListDirect.push_back(_function);
				} else {
					m_callerListDirectInCallback.push_back(_function);
				}
			}
			/**
			 * @brief Check if an object is registered in the Signal
			 * @param[in] _obj shared pointer on the object
			 * @return true The object is connected at this signal.
			 * @return false The object is NOT connected on this signal.
			 */
			bool isRegistered(std::shared_ptr<void> _obj) {
				if (_obj == nullptr) {
					return false;
				}
				for (auto &it : m_callerList) {
					std::shared_ptr<void> obj = it.first.lock();
					if (obj == _obj) {
						return true;
					}
				}
				for (auto &it : m_callerListInCallback) {
					std::shared_ptr<void> obj = it.first.lock();
					if (obj == _obj) {
						return true;
					}
				}
				return false;
			};
			/**
			 * @brief remove link on the signal.
			 * @param[in] _obj shared pointer on the removing object
			 */
			void release(std::shared_ptr<void> _obj);{
				if (m_callInProgress == 0) {
					// Remove from the list :
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				} else {
					// just remove weak poointer
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.lock() == _obj) {
							it->first.reset();
						} else {
							++it;
						}
					}
					m_someOneRemoveInCall = true;
				}
				// remove from add list in callback progress
				auto it = m_callerListInCallback.begin();
				while(it != m_callerListInCallback.end()) {
					if (it->first.lock() == _obj) {
						it = m_callerListInCallback.erase(it);
					} else {
						++it;
					}
				}
			}
			/**
			 * @brief Generate a signal on all interface listening.
			 * @param[in] _data data to emit
			 */
			void emit(const T&... _data);{
				#ifdef DEBUG
					m_signalCallLevel++;
					int32_t tmpID = m_uidSignal++;
				#endif
				m_callInProgress++;
				if (m_periodic == true) {
					ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << /*etk::to_string(_data) <<*/ "' to: " << m_callerList.size() << " element(s)");
				} else {
					ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << /*etk::to_string(_data) <<*/ "' to: " << m_callerList.size() << " element(s)");
				}
				{
					auto it(m_callerList.begin());
					while (it != m_callerList.end()) {
						std::shared_ptr<void> destObject = it->first.lock();
						if (destObject == nullptr) {
							it = m_callerList.erase(it);
							ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "    nullptr dest");
							continue;
						}
						if (m_periodic == true) {
							ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						it->second(_data...);
						++it;
					}
				}
				{
					auto it(m_callerListDirect.begin());
					while (it != m_callerListDirect.end()) {
						if (m_periodic == true) {
							ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						} else {
							ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
						}
						(*it)(_data...);
						++it;
					}
				}
				m_callInProgress--;
				#ifdef DEBUG
					m_signalCallLevel--;
				#endif
				// Remove element in call phase:
				if (m_someOneRemoveInCall == true) {
					m_someOneRemoveInCall = false;
					// Remove from the list :
					auto it(m_callerList.begin());
					while(it != m_callerList.end()) {
						if (it->first.expired() == true) {
							it = m_callerList.erase(it);
						} else {
							++it;
						}
					}
				}
				// add element in call phase:
				if (m_callerListInCallback.size() > 0) {
					for (auto &it : m_callerListInCallback) {
						m_callerList.push_back(it);
					}
					m_callerListInCallback.clear();
				}
				if (m_callerListDirectInCallback.size() > 0) {
					for (auto &it : m_callerListDirectInCallback) {
						m_callerListDirect.push_back(it);
					}
					m_callerListDirectInCallback.clear();
				}
			}
			size_t getNumberConnected();{
				return m_callerList.size() + m_callerListDirect.size();
			}
	};
	#endif
	#undef __class__
	#define __class__ nullptr
}


//#define SDFGSDFGSDFGSDFGSDFGSDFG
#ifdef SDFGSDFGSDFGSDFGSDFGSDFG
#include <functional>
#include <type_traits>
#include <utility>

template<int I> struct placeholder{};

namespace std{
	template<int I>
	struct is_placeholder< ::placeholder<I>> : std::integral_constant<int, I>{
		
	};
} // std::

namespace detail{
	template<std::size_t... Is, class F, class... Args>
	auto easy_bind(indices<Is...>, F const& f, Args&&... args) -> decltype(std::bind(f, std::forward<Args>(args)..., placeholder<Is + 1>{}...)) {
		return std::bind(f, std::forward<Args>(args)..., placeholder<Is + 1>{}...);
	}
} // detail::

template<class R, class... FArgs, class... Args>
auto easy_bind(std::function<R(FArgs...)> const& f, Args&&... args) -> decltype(detail::easy_bind(build_indices<sizeof...(FArgs) - sizeof...(Args)>{}, f, std::forward<Args>(args)...)) {
	return detail::easy_bind(build_indices<sizeof...(FArgs) - sizeof...(Args)>{}, f, std::forward<Args>(args)...);
}
#endif

