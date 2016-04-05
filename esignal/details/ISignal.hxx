/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/ISignal.h>
#include <esignal/details/Signal.hxx>

template<class... T_ARGS>
esignal::ISignal<T_ARGS...>::ISignal(esignal::Interface* _signalInterfaceLink,
        const std::string& _name,
        const std::string& _description):
  m_signalInterfaceLink(_signalInterfaceLink),
  m_name(_name),
  m_description(_description) {
	// add a reference on the current signal ...
	if (m_signalInterfaceLink != nullptr) {
		m_signalInterfaceLink->signals.add(this);
	}
}

template<class... T_ARGS>
esignal::ISignal<T_ARGS...>::~ISignal() {
	if (m_signalInterfaceLink != nullptr) {
		m_signalInterfaceLink->signals.remove(this);
	}
}

template<class... T_ARGS>
const std::string& esignal::ISignal<T_ARGS...>::getName() const {
	return m_name;
}

template<class... T_ARGS>
const std::string& esignal::ISignal<T_ARGS...>::getDescription() const {
	return m_description;
}

