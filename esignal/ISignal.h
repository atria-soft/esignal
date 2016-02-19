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
	#undef __class__
	#define __class__ "ISignal<T>"
	template<class... Args>
	class ISignal : public Signal<Args...> {
		protected:
			esignal::Interface& m_signalInterfaceLink;
			std::string m_name;
			std::string m_description;
		public:
			/**
			 * @brief Create a signal with a specific type.
			 * @param[in] _signalInterfaceLink reference on the signal lister.
			 * @param[in] _name Static name of the signal.
			 * @param[in] _description Description of the signal.
			 */
			ISignal(esignal::Interface& _signalInterfaceLink,
			        const std::string& _name,
			        const std::string& _description = ""):
			  m_signalInterfaceLink(_signalInterfaceLink),
			  m_name(_name),
			  m_description(_description) {
				// add a reference on the current signal ...
				m_signalInterfaceLink.signalAdd(this);
			}
			/**
			 * @brief Destructor.
			 */
			virtual ~ISignal() {
				m_signalInterfaceLink.signalRemove(this);
			}
			const std::string& getName() const {
				return m_name;
			}
			const std::string& getDescription() const {
				return m_description;
			}
	};
	#undef __class__
	#define __class__ nullptr
}

