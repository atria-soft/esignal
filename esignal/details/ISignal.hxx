/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/ISignal.h>
#include <esignal/details/Signal.hxx>

template<class... Args>
esignal::ISignal<Args...>::ISignal(esignal::Interface& _signalInterfaceLink,
        const std::string& _name,
        const std::string& _description):
  m_signalInterfaceLink(_signalInterfaceLink),
  m_name(_name),
  m_description(_description) {
	// add a reference on the current signal ...
	m_signalInterfaceLink.signalAdd(this);
}

template<class... Args>
esignal::ISignal<Args...>::~ISignal() {
	m_signalInterfaceLink.signalRemove(this);
}

template<class... Args>
const std::string& esignal::ISignal<Args...>::getName() const {
	return m_name;
}

template<class... Args>
const std::string& esignal::ISignal<Args...>::getDescription() const {
	return m_description;
}


