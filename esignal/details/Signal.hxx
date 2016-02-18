/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/Signal.h>
#include <functional>
#include <memory>

#undef __class__
#define __class__ "Signal<T>"
#if 0
template<typename... T> esignal::Signal<T...>::Signal() :
  m_callInProgress(0),
  m_someOneRemoveInCall(false) {
	
}

template<typename... T> esignal::Signal<T...>::~Signal() {
	
}

template<typename... T> void esignal::Signal<T...>::connect(std::shared_ptr<void> _obj, std::function<void(const T&...)> _function ) {
	if (m_callInProgress == 0) {
		m_callerList.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
	} else {
		m_callerListInCallback.push_back(std::make_pair(std::weak_ptr<void>(_obj), _function));
	}
}

template<typename... T> void esignal::Signal<T...>::connect(std::function<void(const T&...)> _function ) {
	if (m_callInProgress == 0) {
		m_callerListDirect.push_back(_function);
	} else {
		m_callerListDirectInCallback.push_back(_function);
	}
}

template<typename... T> bool esignal::Signal<T...>::isRegistered(std::shared_ptr<void> _obj) {
	if (_obj == nullptr) {
		return false;
	}
	for (auto &it : m_callerList) {
		std::shared_ptr<void> obj = it.first.lock();
		if (obj == _obj) {
			return true;
		}
	}
	for (auto &it : m_callerListInCallback) {
		std::shared_ptr<void> obj = it.first.lock();
		if (obj == _obj) {
			return true;
		}
	}
	return false;
}

template<typename... T> void esignal::Signal<T...>::release(std::shared_ptr<void> _obj) {
	if (m_callInProgress == 0) {
		// Remove from the list :
		auto it(m_callerList.begin());
		while(it != m_callerList.end()) {
			if (it->first.lock() == _obj) {
				it = m_callerList.erase(it);
			} else {
				++it;
			}
		}
	} else {
		// just remove weak poointer
		auto it(m_callerList.begin());
		while(it != m_callerList.end()) {
			if (it->first.lock() == _obj) {
				it->first.reset();
			} else {
				++it;
			}
		}
		m_someOneRemoveInCall = true;
	}
	// remove from add list in callback progress
	auto it = m_callerListInCallback.begin();
	while(it != m_callerListInCallback.end()) {
		if (it->first.lock() == _obj) {
			it = m_callerListInCallback.erase(it);
		} else {
			++it;
		}
	}
}

template<typename... T> void esignal::Signal<T...>::emit(const T&... _data) {
	#ifdef DEBUG
		m_signalCallLevel++;
		int32_t tmpID = m_uidSignal++;
	#endif
	m_callInProgress++;
	if (m_periodic == true) {
		ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << /*etk::to_string(_data) <<*/ "' to: " << m_callerList.size() << " element(s)");
	} else {
		ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "emit signal{" << tmpID << "} : signal='" << m_name << "' data='" << /*etk::to_string(_data) <<*/ "' to: " << m_callerList.size() << " element(s)");
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
			it->second(_data...);
			++it;
		}
	}
	{
		auto it(m_callerListDirect.begin());
		while (it != m_callerListDirect.end()) {
			if (m_periodic == true) {
				ESIGNAL_VERBOSE(esignal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
			} else {
				ESIGNAL_DEBUG(esignal::logIndent(m_signalCallLevel-1) << "X     signal{" << tmpID << "} :");// [" << destObject->getId() << "]" << destObject->getObjectType());
			}
			(*it)(_data...);
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
			m_callerList.push_back(it);
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

template<typename... T> size_t esignal::Signal<T...>::getNumberConnected() {
	return m_callerList.size() + m_callerListDirect.size();
}
#endif





