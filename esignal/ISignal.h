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
#include <esignal/Signal.h>
#include <esignal/Interface.h>

namespace esignal {
	/**
	 * @brief Sigla same as @ref esignal::Signal withe a name and a description to manage a list of signals.
	 */
	template<class... T_ARGS>
	class ISignal : public Signal<T_ARGS...> {
		protected:
			esignal::Interface& m_signalInterfaceLink; //!< interface of the signal manager.
			std::string m_name; //!< name of the signal.
			std::string m_description; //!< description of the signal.
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _countObs (optionnal) Local observer to know the count of connection on the signal.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 */
			template<class CLASS_TYPE, class FUNC_TYPE>
			ISignal(CLASS_TYPE* _signalInterfaceLink,
			        FUNC_TYPE _func,
			        const std::string& _name,
			        const std::string& _description = "");
			ISignal(esignal::Interface& _signalInterfaceLink,
			        const std::string& _name,
			        const std::string& _description = "");
			/**
			 * @brief Destructor.
			 */
			virtual ~ISignal();
			virtual const std::string& getName() const;
			virtual const std::string& getDescription() const;
	};
}

template<class... T_ARGS>
template<class CLASS_TYPE, class FUNC_TYPE>
esignal::ISignal<T_ARGS...>::ISignal(CLASS_TYPE* _class,
                                     FUNC_TYPE _func,
                                     const std::string& _name,
                                     const std::string& _description) :
  esignal::Signal<T_ARGS...>(_class, _func),
  m_signalInterfaceLink(*_class),
  m_name(_name),
  m_description(_description) {
	// add a reference on the current signal ...
	m_signalInterfaceLink.signalAdd(this);
}

