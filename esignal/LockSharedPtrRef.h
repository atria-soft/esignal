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
	/**
	 * @brief shared ptr that permeit to lock access of the internal data (it does not manage the allication and remove of the data).
	 * @todo Change this with atomic_shared_ptr<> when availlable.
	 * @input[in] TYPE Type of the internal data
	 */
	template<class TYPE>
	class LockSharedPtrRef {
		public:
			RefCount<TYPE>* m_counter; //!< Access on the reference counter
		public:
			/**
			 * @brief Basic contructor (with the object to ref count)
			 * @param[in] _pointer Pointer on the data (default nullptr)
			 */
			LockSharedPtrRef(TYPE* _pointer=nullptr) :
			  m_counter(nullptr) {
				if (_pointer != nullptr) {
					m_counter = new RefCount<TYPE>(_pointer);
					m_counter->inc();
				}
			}
			/**
			 * @brief Copy contructor
			 * @param[in] _obj object to copy
			 */
			LockSharedPtrRef(const LockSharedPtrRef<TYPE>& _obj) :
			  m_counter(_obj.m_counter) {
				if (m_counter == nullptr) {
					return;
				}
				m_counter->inc();
			}
			/**
			 * @brief Copy operator (It copy the counter and increment the it).
			 * @param[in] _obj objetc to copy.
			 * @return Reference of this
			 */
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
			/**
			 * @brief Contructor (move)
			 * @param[in] _obj move object
			 */
			LockSharedPtrRef(LockSharedPtrRef<TYPE>&& _obj) :
			  m_counter(std::move(_obj.m_counter)) {
				
			}
			/**
			 * @brief Copy operator (force move) ==> removed
			 */
			LockSharedPtrRef& operator=(LockSharedPtrRef<TYPE>&& _obj) = delete;
			/**
			 * @brief Destructor of the class (decrement the counter and remove it if it is the last one...)
			 */
			~LockSharedPtrRef() {
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
			/**
			 * @brief Remove the data on the conter reference (it does not exist anymore)
			 */
			void removeData() {
				if (m_counter != nullptr) {
					m_counter->remove();
				}
			}
			/**
			 * @brief Call disconnect on the parameter class with lock prevention
			 * @param[in] _uid ID to dicsonnect on the sub element
			 */
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
			/**
			 * @brief Check if the value is availlable
			 * @return true The data is availlable
			 * @return false The data has been removed
			 */
			bool isAlive() {
				return m_counter != nullptr;
			}
	};
}

