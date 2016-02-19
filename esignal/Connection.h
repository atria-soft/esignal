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
	
	class Connection {
		public:
			Connection():
			  m_signalRefUnique(), m_uid(0) {
				
			}
			Connection(LockSharedPtrRef<Base> _ref, std::size_t _id):
			  m_signalRefUnique(_ref), m_uid(_id) {
				
			}
			Connection(Connection&&) = default; // movable
			Connection& operator=(Connection&&) = default; // movable op
			Connection(const Connection&) = delete; // not copyable
			Connection& operator=(const Connection&) = delete; // no copy operator
			/*
			void set(Connection&& _obj) {
				disconnect();
				std::swap(m_signalRefUnique, _obj.m_signalRefUnique);
				std::swap(m_uid,_obj.m_uid);
			}
			*/
			~Connection() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
			void disconnect() {
				m_signalRefUnique.disconnect(m_uid);
				m_uid = 0;
			}
			bool isConnected() {
				return m_signalRefUnique.isAlive();
			}
		private:
			LockSharedPtrRef<Base> m_signalRefUnique;
			std::size_t m_uid;
	};
}

