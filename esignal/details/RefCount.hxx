/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/RefCount.h>

template<class TYPE>
esignal::RefCount<TYPE>::RefCount(TYPE* _data) :
  m_count(0),
  m_data(_data) {
	// nothing to do.
}

template<class TYPE>
esignal::RefCount<TYPE>::~RefCount() {
	m_data = nullptr;
}

template<class TYPE>
void esignal::RefCount<TYPE>::lock() {
	m_lock.lock();
}

template<class TYPE>
void esignal::RefCount<TYPE>::unlock() {
	m_lock.unlock();
}

template<class TYPE>
void esignal::RefCount<TYPE>::inc() {
	lock();
	m_count++;
	unlock();
}

template<class TYPE>
int64_t esignal::RefCount<TYPE>::dec() {
	int64_t val;
	lock();
	m_count--;
	val = m_count;
	unlock();
	return val;
}

template<class TYPE>
int64_t esignal::RefCount<TYPE>::getCount() const {
	return m_count;
}

template<class TYPE>
void esignal::RefCount<TYPE>::remove() {
	lock();
	m_data = nullptr;
	unlock();
}

template<class TYPE>
TYPE* esignal::RefCount<TYPE>::get() {
	return m_data;
}