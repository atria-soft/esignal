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
	#undef __class__
	#define __class__ "Signal<T>"
	class Base {
		protected:
			LockSharedPtrRef<Base> m_shared;
			static size_t s_uid;
		public:
			Base();
			// copy constructor:
			Base(const Base&) = delete;
			// copy operator:
			//Base& operator=(Base) = delete;
			//Base& operator=(const Base& _obj) = delete;
			// Move constructor
			Base(Base&& _obj) = delete;
			// Move operator
			//Base& operator=(Base&& _obj) = delete;
			
			virtual ~Base();
			virtual void disconnect(const std::shared_ptr<void>& _obj);
			virtual void disconnect(std::size_t _uid) = 0;
			
			virtual const std::string& getName() const;
			virtual const std::string& getDescription() const;
	};
	std::ostream& operator <<(std::ostream& _os, const esignal::Base& _obj);
	const char* logIndent(int32_t _iii);
}
