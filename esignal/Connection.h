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

namespace esignal {
	/**
	 * @brief connection on the signal (disconnect it whe removed)
	 */
	class Connection {
		public:
			//! @brief Constructor (no link)
			Connection();
			//! @brief Constructor (link)
			Connection(const esignal::LockSharedPtrRef<esignal::Base>& _ref, std::size_t _id);
			//! @brief Move Constructor
			Connection(Connection&& _obj);
			//! @brief Move operator.
			Connection& operator=(Connection&& _obj);
			//! @brief Copy constructor (REMOVED)
			Connection(const Connection&) = delete;
			//! @brief Copy operator (REMOVED)
			Connection& operator=(const Connection&) = delete;
			//! @brief Destructor.
			~Connection();
			//! @brief Disconnect the signal.
			void disconnect();
			/**
			 * @brief Check if the connection is alive or signal removed
			 * @return true The signal is connected.
			 * @return false The signal is NOT connected.
			 */
			bool isConnected();
		private:
			esignal::LockSharedPtrRef<esignal::Base> m_signalRefUnique; //!< reference on the Signal.
			std::size_t m_uid; //!< UID of the current connection.
	};
}

