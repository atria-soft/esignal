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
	template<class... Args>
	class ISignal : public Signal<Args...> {
		protected:
			esignal::Interface& m_signalInterfaceLink; //!< interface of the signal manager.
			std::string m_name; //!< name of the signal.
			std::string m_description; //!< description of the signal.
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 */
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

