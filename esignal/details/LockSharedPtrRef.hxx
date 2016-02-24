/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esignal/LockSharedPtrRef.h>

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(TYPE* _pointer) :
  m_counter(nullptr) {
	if (_pointer != nullptr) {
		m_counter = new RefCount<TYPE>(_pointer);
		m_counter->inc();
	}
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(const LockSharedPtrRef<TYPE>& _obj) :
  m_counter(_obj.m_counter) {
	if (m_counter == nullptr) {
		return;
	}
	m_counter->inc();
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>& esignal::LockSharedPtrRef<TYPE>::operator=(const esignal::LockSharedPtrRef<TYPE>& _obj) {
	if (&_obj == this) {
		return *this;
	}
	if (m_counter != nullptr) {
		m_counter->dec();
		m_counter = nullptr;
	}
	m_counter = _obj.m_counter;
	if (m_counter == nullptr) {
		return *this;
	}
	m_counter->inc();
	return *this;
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::LockSharedPtrRef(LockSharedPtrRef<TYPE>&& _obj) :
  m_counter(std::move(_obj.m_counter)) {
	
}

template<class TYPE>
esignal::LockSharedPtrRef<TYPE>::~LockSharedPtrRef() {
	if (m_counter == nullptr) {
		return;
	}
	int64_t count = m_counter->dec();
	if (count > 0) {
		return;
	}
	delete m_counter;
	m_counter = nullptr;
}

template<class TYPE>
void esignal::LockSharedPtrRef<TYPE>::removeData() {
	if (m_counter != nullptr) {
		m_counter->remove();
	}
}

template<class TYPE>
void esignal::LockSharedPtrRef<TYPE>::disconnect(std::size_t _uid) {
	if (m_counter == nullptr) {
		return;
	}
	m_counter->lock();
	TYPE* val = m_counter->get();
	if (val != nullptr) {
		val->disconnect(_uid);
	}
	m_counter->unlock();
}

template<class TYPE>
bool esignal::LockSharedPtrRef<TYPE>::isAlive() {
	return m_counter != nullptr;
}



