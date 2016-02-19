/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <functional>
#include <memory>
#include <esignal/debug.h>
#include <esignal/Base.h>
#include <esignal/RefCount.h>
#include <functional>
#include <type_traits>
#include <utility>
#include <mutex>

namespace esignal {
	extern size_t s_uid;
	
	template<class TYPE>
	class LockSharedPtrRef {
		public:
			RefCount<TYPE>* m_counter;
		public:
			LockSharedPtrRef(TYPE* _pointer=nullptr) :
			  m_counter(nullptr) {
				if (_pointer != nullptr) {
					m_counter = new RefCount<TYPE>(_pointer);
					m_counter->inc();
				}
			}
			// copy constructor:
			LockSharedPtrRef(const LockSharedPtrRef<TYPE>& _obj) :
			  m_counter(_obj.m_counter) {
				if (m_counter == nullptr) {
					return;
				}
				m_counter->inc();
			}
			// copy operator:
			//LockSharedPtrRef& operator=(LockSharedPtrRef<TYPE>) = delete;
			LockSharedPtrRef& operator=(const LockSharedPtrRef<TYPE>& _obj) {
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
			// Move constructor
			LockSharedPtrRef(LockSharedPtrRef<TYPE>&& _obj) :
			  m_counter(std::move(_obj.m_counter)) {
				
			}
			// Move operator
			#if 1
				LockSharedPtrRef& operator=(LockSharedPtrRef<TYPE>&& _obj) = delete;
			#else
				LockSharedPtrRef& operator=(LockSharedPtrRef<TYPE>&& _obj) {
					m_counter = std::move(_obj.m_counter);
				}
			#endif
			~LockSharedPtrRef() {
				int64_t count = m_counter->dec();
				if (count > 0) {
					return;
				}
				delete m_counter;
				m_counter = nullptr;
			}
			void removeData() {
				if (m_counter != nullptr) {
					m_counter->remove();
				}
			}
			void disconnect(std::size_t _uid) {
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
			bool isAlive() {
				return m_counter != nullptr;
			}
	};
}

