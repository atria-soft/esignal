/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <functional>
#include <ememory/memory.hpp>
#include <esignal/debug.hpp>
#include <esignal/Base.hpp>

namespace esignal {
	/**
	 * @brief connection on the signal (disconnect it whe removed)
	 */
	class Connection {
		private:
			ememory::WeakPtr<esignal::BaseInternal> m_data;
			size_t m_uid; //!< UID of the current connection.
		public:
			/**
			 * @brief Constructor (no link)
			 */
			Connection();
			/**
			 * @brief Constructor (link)
			 * @param[in] _ref Reference ID of the Signal extern handle
			 * @param[in] _id Id of the Connection handle
			 */
			Connection(ememory::SharedPtr<esignal::BaseInternal> _ref, size_t _id);
			/**
			 * @brief Move Constructor
			 * @param[in] _obj Connection Object to move
			 */
			Connection(Connection&& _obj);
			/**
			 * @brief Move operator.
			 * @param[in] _obj Connection Object to move
			 * @return Local reference on the local object (moved)
			 */
			Connection& operator=(Connection&& _obj);
			/**
			 * @brief Copy constructor (REMOVED)
			 */
			Connection(const Connection&) = delete;
			/**
			 * @brief Copy operator (REMOVED)
			 * @return Reference on this
			 */
			Connection& operator=(const Connection&) = delete;
			/**
			 * @brief Destructor.
			 */
			~Connection();
			/**
			 * @brief Disconnect the signal.
			 */
			void disconnect();
			/**
			 * @brief Check if the connection is alive or signal removed
			 * @return true The signal is connected.
			 * @return false The signal is NOT connected.
			 */
			bool isConnected();
	};
}

