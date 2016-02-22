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

size_t esignal::Base::s_uid = 0;

esignal::Base::Base() :
  m_shared(this) {
	
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


std::ostream& esignal::operator <<(std::ostream& _os, const esignal::Base& _obj) {
	_os << _obj.getName();
	return _os;
}

void esignal::Base::disconnectShared(const std::shared_ptr<void>& _obj) {
	
}

const char* esignal::logIndent(int32_t _iii) {
	static const char g_val[] = "                    ";
	if (_iii > 5) {
		return g_val;
	}
	return g_val + (5-_iii)*4;
}
