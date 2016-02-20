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
			Connection():
			  m_signalRefUnique(), m_uid(0) {
				
			}
			//! @brief Constructor (link)
			Connection(const LockSharedPtrRef<Base>& _ref, std::size_t _id):
			  m_signalRefUnique(_ref),
			  m_uid(_id) {
				
			}
			//! @brief Move Constructor
			Connection(Connection&& _obj):
			  m_signalRefUnique(_obj.m_signalRefUnique),
			  m_uid(_obj.m_uid) {
				_obj.m_uid = 0;
			}
			//! @brief Move operator.
			Connection& operator=(Connection&& _obj) {
				disconnect();
				m_signalRefUnique = _obj.m_signalRefUnique;
				m_uid = _obj.m_uid;
				_obj.m_uid = 0;
				return *this;
			}
			//! @brief Copy constructor (REMOVED)
			Connection(const Connection&) = delete;
			//! @brief Copy operator (REMOVED)
			Connection& operator=(const Connection&) = delete;
			//! @brief Destructor.
			~Connection() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
			//! @brief Disconnect the signal.
			void disconnect() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
			/**
			 * @brief Check if the connection is alive or signal removed
			 * @return true The signal is connected.
			 * @return false The signal is NOT connected.
			 */
			bool isConnected() {
				return m_signalRefUnique.isAlive();
			}
		private:
			LockSharedPtrRef<Base> m_signalRefUnique; //!< reference on the Signal.
			std::size_t m_uid; //!< UID of the current connection.
	};
}

