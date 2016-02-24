/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

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
			RefCount(TYPE* _data);
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
			~RefCount();
		public:
			//!< @brief Lock the interface
			void lock();
			//!< @brief Unlock the interface
			void unlock();
			//!< @brief Increment the ref-counting
			void inc();
			//!< @brief Decrement the ref-counting
			int64_t dec();
			//!< @brief Get number of connected
			int64_t getCount() const;
			//!< @brief Remove the data
			void remove();
			//!< @brief Get the recoreded data
			TYPE* get();
	};
}
