/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/Signal.h>

template<class... T_ARGS>
esignal::Signal<T_ARGS...>::~Signal() {
	if (m_signalInterfaceLink != nullptr) {
		m_signalInterfaceLink->signals.remove(this);
	}
}

template<typename... T_ARGS>
esignal::Signal<T_ARGS...>::Signal(esignal::BaseInternal::ObserverConnection _countObs,
                                   const std::string& _name,
                                   const std::string& _description):
  esignal::Base(),
  m_signalInterfaceLink(nullptr) {
	// create internal data assiciated:
	m_data = ememory::makeShared<esignal::SignalInternal<T_ARGS...>>(std::move(_countObs));
	if (esignal::Base::m_data != nullptr) {
		esignal::Base::m_data->setName(_name);
		esignal::Base::m_data->setDescription(_description);
	}
}

template<class... T_ARGS>
esignal::Signal<T_ARGS...>::Signal(esignal::Interface* _signalInterfaceLink,
                                   const std::string& _name,
                                   const std::string& _description):
  m_signalInterfaceLink(_signalInterfaceLink) {
	// create internal data assiciated:
	m_data = ememory::makeShared<esignal::SignalInternal<T_ARGS...>>(nullptr);
	if (esignal::Base::m_data != nullptr) {
		esignal::Base::m_data->setName(_name);
		esignal::Base::m_data->setDescription(_description);
	}
	// add a reference on the current signal ...
	if (m_signalInterfaceLink != nullptr) {
		m_signalInterfaceLink->signals.add(this);
	}
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::emit(const T_ARGS&... _args) {
	#ifdef DEBUG
		int32_t tmpID = s_uidSignalEmit++;
	#endif
	// TODO : Add protection ... but how ...
	m_callInProgress++;
	if (m_periodic == false) {
		ESIGNAL_DEBUG(esignal::logIndent(m_callInProgress-1) << "     signal{" << tmpID << "} : '" << getName() << "' ***/" << m_executors.size());
		for (size_t iii=0; iii < m_executors.size(); ++iii) {
			ESIGNAL_VERBOSE(esignal::logIndent(m_callInProgress-1) << "         {" << tmpID << "} : " << iii);
			m_executors[iii]->emit(_args...);
		}
	} else {
		for (size_t iii=0; iii < m_executors.size(); ++iii) {
			m_executors[iii]->emit(_args...);
		}
	}
	if (m_callInProgress == 1) {
		bool haveRemove = false;
		auto it = m_executors.begin();
		while (it != m_executors.end()) {
			if (    *it == nullptr
			     || (*it)->m_removed == true) {
				it = m_executors.erase(it);
				haveRemove = true;
				continue;
			}
			++it;
		}
		if (haveRemove == true) {
			if (m_connectionObserver!=nullptr) {
				m_connectionObserver(m_executors.size());
			}
		}
	}
	m_callInProgress--;
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::removeIfPossible() {
	if (m_callInProgress != 0) {
		return;
	}
	bool haveRemove = false;
	auto it = m_executors.begin();
	while (it != m_executors.end()) {
		if (    *it == nullptr
		     || (*it)->m_removed == true) {
			it = m_executors.erase(it);
			haveRemove = true;
			continue;
		}
		++it;
	}
	if (haveRemove == true) {
		if (m_connectionObserver!=nullptr) {
			m_connectionObserver(m_executors.size());
		}
	}
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::disconnect(size_t _uid) {
	for (size_t iii=0; iii < m_executors.size(); ++iii) {
		if (m_executors[iii]->m_uid == _uid) {
			m_executors[iii]->m_removed = true;
			break;
		}
	}
	removeIfPossible();
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::disconnectShared(const ememory::SharedPtr<void>& _obj) {
	for (size_t iii=0; iii < m_executors.size(); ++iii) {
		if (m_executors[iii]->isSharedPtr(_obj) == true) {
			m_executors[iii]->m_removed = true;
		}
	}
	removeIfPossible();
}

template<typename... T_ARGS>
size_t esignal::SignalInternal<T_ARGS...>::size() const {
	return m_executors.size();
}

template<typename... T_ARGS>
bool esignal::SignalInternal<T_ARGS...>::empty() const {
	return m_executors.empty();
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::clear() {
	for (size_t iii=0; iii < m_executors.size(); ++iii) {
		m_executors[iii]->m_removed = true;
	}
	removeIfPossible();
}


template<typename... T_ARGS>
esignal::SignalInternal<T_ARGS...>::Executor::Executor(Observer&& _observer):
  m_removed(false),
  m_uid(0) {
	m_uid = s_uid++;
	m_observer = std::move(_observer);
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::Executor::emit(const T_ARGS&... _values) {
	if (m_removed == true) {
		return;
	}
	try {
		m_observer(_values...);
	} catch(...) {
		m_removed = true;
	}
}

template<typename... T_ARGS>
bool esignal::SignalInternal<T_ARGS...>::Executor::isSharedPtr(const ememory::SharedPtr<void>& _obj) {
	return false;
}




template<typename... T_ARGS>
esignal::SignalInternal<T_ARGS...>::ExecutorShared::ExecutorShared(ememory::WeakPtr<void> _object, Observer&& _observer) :
  Executor(std::move(_observer)),
  m_object(_object) {
	
}

template<typename... T_ARGS>
void esignal::SignalInternal<T_ARGS...>::ExecutorShared::emit(const T_ARGS&... _values) {
	// TODO: maybe an error if the object is not manage by the same thread.
	ememory::SharedPtr<void> destObject = m_object.lock();
	if (destObject == nullptr) {
		Executor::m_removed = true;
		return;
	}
	if (Executor::m_removed == true) {
		return;
	}
	try {
		Executor::m_observer(_values...);
	} catch(...) {
		Executor::m_removed = true;
	}
}

template<typename... T_ARGS>
bool esignal::SignalInternal<T_ARGS...>::ExecutorShared::isSharedPtr(const ememory::SharedPtr<void>& _obj) {
	ememory::SharedPtr<void> destObject = m_object.lock();
	if (destObject == nullptr) {
		return true;
	}
	if (destObject == _obj) {
		return true;
	}
	return false;
}
