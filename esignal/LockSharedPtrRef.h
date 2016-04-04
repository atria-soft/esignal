/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <esignal/RefCount.h>

namespace esignal {
	/**
	 * @brief shared ptr that permeit to lock access of the internal data (it does not manage the allication and remove of the data).
	 * @todo Change this with atomic_shared_ptr<> when availlable.
	 * @param TYPE Type of the internal data
	 */
	template<class TYPE>
	class LockSharedPtrRef {
		public:
			// TODO : Remove data from refcounter ...==>
			// esignal::RefCount* m_counter; //!< Access on the reference counter
			// TYPE* m_data; //!< Pointer on the data
			esignal::RefCount<TYPE>* m_counter; //!< Access on the reference counter
		public:
			/**
			 * @brief Basic contructor (with the object to ref count)
			 * @param[in] _pointer Pointer on the data (default nullptr)
			 */
			LockSharedPtrRef(TYPE* _pointer=nullptr);
			/**
			 * @brief Copy contructor
			 * @param[in] _obj object to copy
			 */
			LockSharedPtrRef(const LockSharedPtrRef<TYPE>& _obj);
			/**
			 * @brief Copy operator (It copy the counter and increment the it).
			 * @param[in] _obj objetc to copy.
			 * @return Reference of this
			 */
			LockSharedPtrRef& operator=(const LockSharedPtrRef<TYPE>& _obj);
			/**
			 * @brief Contructor (move)
			 * @param[in] _obj move object
			 */
			LockSharedPtrRef(LockSharedPtrRef<TYPE>&& _obj);
			/**
			 * @brief Copy operator (force move) ==> removed
			 */
			LockSharedPtrRef& operator=(LockSharedPtrRef<TYPE>&& _obj) = delete;
			/**
			 * @brief Destructor of the class (decrement the counter and remove it if it is the last one...)
			 */
			~LockSharedPtrRef();
			/**
			 * @brief Remove the data on the conter reference (it does not exist anymore)
			 */
			void removeData();
			/**
			 * @brief Call disconnect on the parameter class with lock prevention
			 * @param[in] _uid ID to dicsonnect on the sub element
			 */
			void disconnect(size_t _uid);
			/**
			 * @brief Check if the value is availlable
			 * @return true The data is availlable
			 * @return false The data has been removed
			 */
			bool isAlive();
		private:
			/**
			 * @remove reference on the refcounter...
			 */
			void rmShared();
	};
}

