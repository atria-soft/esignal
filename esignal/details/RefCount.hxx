/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/RefCount.h>
#include <esignal/debug.h>

template<class TYPE>
esignal::RefCount<TYPE>::RefCount(TYPE* _data) :
  m_count(0),
  m_data(_data) {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount New : " << int64_t(_data));
	// nothing to do.
}

template<class TYPE>
esignal::RefCount<TYPE>::~RefCount() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount delete");
	m_data = nullptr;
}

template<class TYPE>
void esignal::RefCount<TYPE>::lock() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount LOCK [START]");
	m_lock.lock();
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount LOCK [STOP]");
}

template<class TYPE>
void esignal::RefCount<TYPE>::unlock() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount UN-LOCK [START]");
	m_lock.unlock();
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount UN-LOCK [STOP]");
}

template<class TYPE>
void esignal::RefCount<TYPE>::inc() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount INC [START] " << m_count);
	lock();
	m_count++;
	unlock();
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount INC [STOP] " << m_count);
}

template<class TYPE>
int64_t esignal::RefCount<TYPE>::dec() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount DEC [START] " << m_count);
	int64_t val;
	lock();
	m_count--;
	val = m_count;
	unlock();
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount DEC [STOP] " << m_count);
	return val;
}

template<class TYPE>
int64_t esignal::RefCount<TYPE>::getCount() const {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount get count " << m_count);
	return m_count;
}

template<class TYPE>
void esignal::RefCount<TYPE>::remove() {
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount remove [START]");
	lock();
	m_data = nullptr;
	unlock();
	ESIGNAL_VERBOSE(int64_t(this) << " RefCount remove [STOP]");
}

template<class TYPE>
TYPE* esignal::RefCount<TYPE>::get() {
	return m_data;
}