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
	
	template<class TYPE>
	class RefCount {
		public:
			std::mutex m_lock;
			int64_t m_count;
			TYPE* m_data;
		public:
			RefCount(TYPE* _data) :
			  m_count(0),
			  m_data(_data) {
				// nothing to do.
			}
			// copy constructor:
			RefCount(const RefCount&) = delete;
			// copy operator:
			RefCount& operator=(RefCount) = delete;
			RefCount& operator=(const RefCount& _obj) = delete;
			// Move constructor
			RefCount(RefCount&& _obj) = delete;
			// Move operator
			RefCount& operator=(RefCount&& _obj) = delete;
		public:
			~RefCount() {
				m_data = nullptr;
			}
		public:
			void lock() {
				m_lock.lock();
			}
			void unlock() {
				m_lock.unlock();
			}
			void inc() {
				lock();
				m_count++;
				unlock();
			}
			int64_t dec() {
				int64_t val;
				lock();
				m_count--;
				val = m_count;
				unlock();
				return val;
			}
			int64_t getCount() const {
				return m_count;
			}
			void remove() {
				lock();
				m_data = nullptr;
				unlock();
			}
			TYPE* get() {
				return m_data;
			}
	};
}
