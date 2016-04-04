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
	/**
	 * @brief Base signal interface for esignal::Signal (permit to create abstract list of signals...)
	 */
	class Base {
		public:
			using ObserverConnection = std::function<void(size_t)>; //!< Define an Observer of the number of observer
		protected:
			esignal::LockSharedPtrRef<esignal::Base> m_shared; //!< Reference counter on itself.
			static size_t s_uid; //!< global id of the signal (STATIC)
			static int64_t s_uidSignalEmit; //!< global id to emit counting
			ObserverConnection m_connectionObserver; //!< propriétéry of the connection handle basic
		public:
			//! @brief Basic constructor:
			Base(ObserverConnection _countObs = nullptr);
			//! @brief Copy constructor:
			Base(const Base&) = delete;
			//! @brief Move constructor
			Base(Base&& _obj) = delete;
			
			virtual ~Base();
			/**
			 * @brief Disconnect the shared_ptr form the Signal
			 * @param[in] _obj Link with the object to check
			 */
			virtual void disconnectShared(const std::shared_ptr<void>& _obj) = 0;
			/**
			 * @brief Disconnect an observer of the signal.
			 * @param[in] _uid Unique id of the signal connection.
			 */
			virtual void disconnect(size_t _uid) = 0;
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
	//! @not_in_doc
	std::ostream& operator <<(std::ostream& _os, const esignal::Base& _obj);
	#ifdef DEBUG
		const char* logIndent(int32_t _iii);
	#endif
}
