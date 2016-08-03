/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ememory/memory.h>
#include <esignal/debug.h>
#include <esignal/Interface.h>
#include <esignal/Base.h>

size_t esignal::Base::s_uid = 1;
int64_t esignal::Base::s_uidSignalEmit = 1;

esignal::Base::Base(ObserverConnection _countObs) :
  m_shared(this),
  m_connectionObserver(_countObs) {
	
}
esignal::Base::~Base() {
	m_shared.removeData();
}


const std::string& esignal::Base::getName() const {
	static std::string noValue;
	return noValue;
}

const std::string& esignal::Base::getDescription() const {
	static std::string noValue;
	return noValue;
}

void esignal::Base::setPeriodic(bool _state) {
	m_periodic = _state;
}

std::ostream& esignal::operator <<(std::ostream& _os, const esignal::Base& _obj) {
	_os << _obj.getName();
	return _os;
}

#ifdef DEBUG
	const char* esignal::logIndent(int32_t _iii) {
		static const char g_val[] = "                                                            ";
		if (_iii > 15) {
			return g_val;
		}
		return g_val + (15-_iii)*4;
	}
#endif
