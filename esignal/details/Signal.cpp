/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#include <esignal/details/Signal.hxx>
#include <etk/types.h>
#include <etk/math/Vector2D.h>
#include <etk/math/Vector3D.h>
#include <etk/Color.h>

#undef __class__
#define __class__ "Signal<void>"

esignal::Signal<void>::Signal(esignal::Interface& _signalInterfaceLink,
                              const std::string& _name,
                              const std::string& _description,
                              bool _periodic) :
  esignal::Base(_signalInterfaceLink, _name, _description, _periodic) {
	
}

esignal::Signal<void>::~Signal() {
	
}

void esignal::Signal<void>::connect(std::shared_ptr<void> _obj, std::function<void()> _function ) {
	if (m_callInProgress == 0) {
		m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
	} else {
		m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
	}
}

void esignal::Signal<void>::connect(std::function<void()> _function ) {
	if (m_callInProgress == 0) {
		m_callerListDirect.push_back(_function);
	} else {
		m_callerListDirectInCallback.push_back(_function);
	}
}

void esignal::Signal<void>::release(std::shared_ptr<void> _obj) {
	auto it(m_callerList.begin());
	if (m_callInProgress == 0) {
		// Remove from the list :
		while(it != m_callerList.end()) {
			if (it->first.lock() == _obj) {
				//ESIGNAL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "'");
				it = m_callerList.erase(it);
			} else {
				++it;
			}
		}
	} else {
		// just remove weak poointer
		while(it != m_callerList.end()) {
			if (it->first.lock() == _obj) {
				//ESIGNAL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "' (delayed)");
				it->first.reset();
			} else {
				++it;
			}
		}
		m_someOneRemoveInCall = true;
	}
	// remove from add list in callback progress
	it = m_callerListInCallback.begin();
	while(it != m_callerListInCallback.end()) {
		if (it->first.lock() == _obj) {
			//ESIGNAL_DEBUG(" unbind : " << _obj->getObjectType() << " signal='" << m_name << "' (notActive)");
			it = m_callerListInCallback.erase(it);
		} else {
			++it;
		}
	}
}

void esignal::Signal<void>::emit() {
	#ifdef DEBUG
		m_signalCallLevel++;
		int32_t tmpID = m_uidSignal++;
	#endif
	m_callInProgress++;
	if (m_periodic == true) {
		ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' to: " << m_callerList.size() << " element(s)");
	} else {
		ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' to: " << m_callerList.size() << " element(s)");
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
			it->second();
			++it;
		}
	}
	{
		auto it(m_callerListDirect.begin());
		while (it != m_callerListDirect.end()) {
			if (m_periodic == true) {
				ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
			} else {
				ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
			}
			(*it)();
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
			if (it.first.expired() == false) {
				m_callerList.push_back(it);
			}
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

size_t esignal::Signal<void>::getNumberConnected() {
	return m_callerList.size() + m_callerListDirect.size();;
}

// void generic signal
template class esignal::Signal<void>;
// std generic signal
template class esignal::Signal<bool>;
template class esignal::Signal<std::string>;
#if __CPP_VERSION__ >= 2011
	template class esignal::Signal<std::u32string>;
#endif

template class esignal::Signal<int64_t>;
template class esignal::Signal<int32_t>;
template class esignal::Signal<int16_t>;
template class esignal::Signal<int8_t>;

template class esignal::Signal<uint64_t>;
template class esignal::Signal<uint32_t>;
template class esignal::Signal<uint16_t>;
template class esignal::Signal<uint8_t>;

template class esignal::Signal<float>;
template class esignal::Signal<double>;
// etk generic vetor 2D
template class esignal::Signal<vec2>;
template class esignal::Signal<bvec2>;
template class esignal::Signal<ivec2>;
template class esignal::Signal<uivec2>;
// etk generic vetor 3D
template class esignal::Signal<vec3>;
template class esignal::Signal<bvec3>;
template class esignal::Signal<ivec3>;
template class esignal::Signal<uivec3>;
// etk generic color
template class esignal::Signal<etk::Color<unsigned char,4>>;
template class esignal::Signal<etk::Color<unsigned char,3>>;
template class esignal::Signal<etk::Color<float,4>>;
template class esignal::Signal<etk::Color<float,3>>;

