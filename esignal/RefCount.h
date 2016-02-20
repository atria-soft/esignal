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
#include <utility>
#include <mutex>

namespace esignal {
	/**
	 * @brief Ref counting tool.
	 */
	template<class TYPE>
	class RefCount {
		private:
			std::mutex m_lock; //!< mutex on the refcounting element
			int64_t m_count; //!< number of element connected
			TYPE* m_data; //!< Pointer on the refconting data
		public:
			//!< generic constructor
			RefCount(TYPE* _data) :
			  m_count(0),
			  m_data(_data) {
				// nothing to do.
			}
			//! @brief Copy constructor (REMOVED)
			RefCount(const RefCount&) = delete;
			//! @brief Copy operator (REMOVED)
			RefCount& operator=(RefCount) = delete;
			//! @previous
			RefCount& operator=(const RefCount& _obj) = delete;
			//! @brief Move constructor (REMOVED)
			RefCount(RefCount&& _obj) = delete;
			//! @brief Move operator (REMOVED)
			RefCount& operator=(RefCount&& _obj) = delete;
			//! @brief Destructor
			~RefCount() {
				m_data = nullptr;
			}
		public:
			//!< @brief Lock the interface
			void lock() {
				m_lock.lock();
			}
			//!< @brief Unlock the interface
			void unlock() {
				m_lock.unlock();
			}
			//!< @brief Increment the ref-counting
			void inc() {
				lock();
				m_count++;
				unlock();
			}
			//!< @brief Decrement the ref-counting
			int64_t dec() {
				int64_t val;
				lock();
				m_count--;
				val = m_count;
				unlock();
				return val;
			}
			//!< @brief Get number of connected
			int64_t getCount() const {
				return m_count;
			}
			//!< @brief Remove the data
			void remove() {
				lock();
				m_data = nullptr;
				unlock();
			}
			//!< @brief Get the recoreded data
			TYPE* get() {
				return m_data;
			}
	};
}
