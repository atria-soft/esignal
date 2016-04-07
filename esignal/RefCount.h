/** @file
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
			/**
			 * @brief generic constructor
			 * @param[in] _data Data to reference the Refcounting
			 */
			RefCount(TYPE* _data);
			//! @brief Copy constructor (REMOVED)
			RefCount(const RefCount&) = delete;
			/**
			 * @brief Copy operator (REMOVED)
			 * @return Reference on this
			 */
			RefCount& operator=(RefCount) = delete;
			/**
			 * @brief Copy operator (REMOVED)
			 * @return Reference on this
			 */
			RefCount& operator=(const RefCount&) = delete;
			//! @brief Move constructor (REMOVED)
			RefCount(RefCount&&) = delete;
			/**
			 * @brief Move operator (REMOVED)
			 * @return Reference on this
			 */
			RefCount& operator=(RefCount&&) = delete;
			//! @brief Destructor
			~RefCount();
		public:
			//! @brief Lock the interface
			void lock();
			//! @brief Unlock the interface
			void unlock();
			//! @brief Increment the ref-counting
			void inc();
			/**
			 * @brief Decrement the ref-counting
			 * @return Number of element connected when decrement
			 */
			int64_t dec();
			/**
			 * @brief Get number of connected
			 * @return The number of element connected on it
			 */
			int64_t getCount() const;
			//! @brief Remove the data
			void remove();
			/**
			 * @brief Get the recoreded data
			 * @return The pointer of the data
			 */
			TYPE* get();
	};
}
