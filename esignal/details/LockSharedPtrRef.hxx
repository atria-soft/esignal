/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esignal/LockSharedPtrRef.h>
#include <esignal/debug.h>

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(TYPE* _pointer) :
  m_counter(nullptr) {
	ESIGNAL_VERBOSE("LockSharedPtrRef new [START] " << int64_t(_pointer));
	if (_pointer != nullptr) {
		m_counter = new RefCount<TYPE>(_pointer);
		m_counter->inc();
	}
	ESIGNAL_VERBOSE("LockSharedPtrRef new [STOP]");
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(const LockSharedPtrRef<TYPE>& _obj) :
  m_counter(_obj.m_counter) {
	ESIGNAL_VERBOSE("LockSharedPtrRef new [START] ref ...");
	if (m_counter == nullptr) {
		return;
	}
	m_counter->inc();
	ESIGNAL_VERBOSE("LockSharedPtrRef new [STOP]");
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>& esignal::LockSharedPtrRef<TYPE>::operator=(const esignal::LockSharedPtrRef<TYPE>& _obj) {
	ESIGNAL_VERBOSE("LockSharedPtrRef copy [START]");
	if (&_obj == this) {
		ESIGNAL_VERBOSE("LockSharedPtrRef copy [STOP] auto-copy");
		return *this;
	}
	if (m_counter != nullptr) {
		m_counter->dec();
		m_counter = nullptr;
	}
	m_counter = _obj.m_counter;
	if (m_counter == nullptr) {
		ESIGNAL_VERBOSE("LockSharedPtrRef copy [STOP] no counter");
		return *this;
	}
	m_counter->inc();
	ESIGNAL_VERBOSE("LockSharedPtrRef copy [STOP]");
	return *this;
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(LockSharedPtrRef<TYPE>&& _obj) :
  m_counter(std::move(_obj.m_counter)) {
	ESIGNAL_VERBOSE("LockSharedPtrRef move [DONE]");
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::~LockSharedPtrRef() {
	ESIGNAL_VERBOSE("LockSharedPtrRef delete [START]");
	rmShared();
	ESIGNAL_VERBOSE("LockSharedPtrRef delete [STOP]");
}

template<class TYPE>
void esignal::LockSharedPtrRef<TYPE>::rmShared() {
	ESIGNAL_VERBOSE("LockSharedPtrRef rmShared [START]");
	if (m_counter == nullptr) {
		return;
	}
	int64_t count = m_counter->dec();
	if (count > 0) {
		m_counter = nullptr;
		return;
	}
	delete m_counter;
	m_counter = nullptr;
	ESIGNAL_VERBOSE("LockSharedPtrRef rmShared [STOP]");
}

template<class TYPE>
void esignal::LockSharedPtrRef<TYPE>::removeData() {
	ESIGNAL_VERBOSE("LockSharedPtrRef removeData [START]");
	if (m_counter != nullptr) {
		m_counter->remove();
	}
	ESIGNAL_VERBOSE("LockSharedPtrRef removeData [STOP]");
}

template<class TYPE>
void esignal::LockSharedPtrRef<TYPE>::disconnect(std::size_t _uid) {
	ESIGNAL_VERBOSE("LockSharedPtrRef disconnect [START] " << _uid);
	if (m_counter == nullptr) {
		ESIGNAL_VERBOSE("LockSharedPtrRef disconnect [STOP] No counter");
		return;
	}
	m_counter->lock();
	TYPE* val = m_counter->get();
	if (val != nullptr) {
		val->disconnect(_uid);
	}
	m_counter->unlock();
	rmShared();
	ESIGNAL_VERBOSE("LockSharedPtrRef disconnect [STOP]");
}

template<class TYPE>
bool esignal::LockSharedPtrRef<TYPE>::isAlive() {
	ESIGNAL_VERBOSE("LockSharedPtrRef isAlive [DONE] ");
	return m_counter != nullptr;
}



