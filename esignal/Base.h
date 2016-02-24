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
#include <functional>
#include <type_traits>
#include <utility>
#include <mutex>
#include <esignal/LockSharedPtrRef.h>

namespace esignal {
	class Base {
		protected:
			esignal::LockSharedPtrRef<esignal::Base> m_shared; //!< Reference counter on itself.
			static size_t s_uid; //!< blobal id of the signal (STATIC)
		public:
			//! @brief Basic constructor:
			Base();
			//! @brief Copy constructor:
			Base(const Base&) = delete;
			//! @brief Move constructor
			Base(Base&& _obj) = delete;
			
			virtual ~Base();
			/**
			 * @brief get name of the signal
			 */
			virtual void disconnectShared(const std::shared_ptr<void>& _obj);
			virtual void disconnect(std::size_t _uid) = 0;
			/**
			 * @brief Get name of the signal.
			 * @return requested name.
			 */
			virtual const std::string& getName() const;
			/**
			 * @brief Get decription of the signal.
			 * @return requested decription.
			 */
			virtual const std::string& getDescription() const;
	};
	std::ostream& operator <<(std::ostream& _os, const esignal::Base& _obj);
	const char* logIndent(int32_t _iii);
}
