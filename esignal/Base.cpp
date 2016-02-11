/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <memory>
#include <esignal/debug.h>
#include <esignal/Interface.h>
#include <esignal/Base.h>

#ifdef DEBUG
	int32_t esignal::Base::m_uidSignal = 0;
	int32_t esignal::Base::m_signalCallLevel = 0;
#endif

esignal::Base::Base(esignal::Interface& _signalInterfaceLink,
                    const std::string& _name,
                    const std::string& _description,
                    bool _periodic) :
  m_signalInterfaceLink(_signalInterfaceLink),
  m_name(_name),
  m_description(_description),
  m_callInProgress(0),
  m_someOneRemoveInCall(false),
  m_periodic(_periodic) {
	// add a reference on the current signal ...
	m_signalInterfaceLink.signalAdd(this);
}

std::ostream& esignal::operator <<(std::ostream& _os, const esignal::Base& _obj) {
	_os << _obj.getName();
	return _os;
}


const char* esignal::logIndent(int32_t _iii) {
	static const char g_val[] = "                    ";
	if (_iii > 5) {
		return g_val;
	}
	return g_val + (5-_iii)*4;
}