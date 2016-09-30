/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <ememory/memory.hpp>
#include <esignal/debug.hpp>
#include <esignal/Interface.hpp>
#include <esignal/Base.hpp>

size_t esignal::BaseInternal::s_uid = 1;
int64_t esignal::BaseInternal::s_uidSignalEmit = 1;

void esignal::BaseInternal::setPeriodic(bool _state) {
	m_periodic = _state;
}

const std::string& esignal::BaseInternal::getName() const {
	return m_name;
}

void esignal::BaseInternal::setName(const std::string& _name) {
	m_name = _name;
}

const std::string& esignal::BaseInternal::getDescription() const {
	return m_description;
}

void esignal::BaseInternal::setDescription(const std::string& _desc) {
	m_description = _desc;
}

esignal::Base::Base() :
  m_data(nullptr) {
	
}
esignal::Base::~Base() {
	m_data.reset();
}

const std::string& esignal::Base::getName() const {
	if (m_data != nullptr) {
		return m_data->getName();
	}
	static std::string noValue;
	return noValue;
}

const std::string& esignal::Base::getDescription() const {
	if (m_data != nullptr) {
		return m_data->getDescription();
	}
	static std::string noValue;
	return noValue;
}

void esignal::Base::setPeriodic(bool _state) {
	if (m_data != nullptr) {
		m_data->setPeriodic(_state);
	}
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
