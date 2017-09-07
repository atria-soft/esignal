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
#include <functional>
#include <type_traits>
#include <utility>
#include <ethread/Mutex.hpp>

/**
 * @brief esignal global interface for all signal implementation
 */
namespace esignal {
	class BaseInternal : public ememory::EnableSharedFromThis<esignal::BaseInternal> {
		public:
			using ObserverConnection = etk::Function<void(size_t)>; //!< Define an Observer of the number of observer
		protected:
			bool m_periodic; //!< The signal is periodic ==> no log with this signal ... (no really needed)
			static size_t s_uid; //!< global id of the signal (STATIC)
			static int64_t s_uidSignalEmit; //!< global id to emit counting
			ObserverConnection m_connectionObserver; //!< propriétéry of the connection handle basic
			etk::String m_name; //!< name of the signal.
			etk::String m_description; //!< description of the signal.
		public:
			/**
			 * @brief Basic constructor:
			 * @param[in] _countObs Observer on the number of connection availlable
			 */
			BaseInternal(ObserverConnection _countObs) :
			  m_periodic(false),
			  m_connectionObserver(_countObs) {
				
			}
			virtual ~BaseInternal() = default;
			/**
			 * @brief Disconnect the shared_ptr form the Signal
			 * @param[in] _obj Link with the object to check
			 */
			virtual void disconnectShared(const ememory::SharedPtr<void>& _obj) = 0;
			/**
			 * @brief Disconnect an observer of the signal.
			 * @param[in] _uid Unique id of the signal connection.
			 */
			virtual void disconnect(size_t _uid) = 0;
			/**
			 * @brief Get name of the signal.
			 * @return requested name.
			 */
			const etk::String& getName() const;
			/**
			 * @brief Set name of the signal.
			 * @param[in] _name new name.
			 */
			void setName(const etk::String& _name);
			/**
			 * @brief Get decription of the signal.
			 * @return requested decription.
			 */
			const etk::String& getDescription() const;
			/**
			 * @brief Set decription of the signal.
			 * @param[in] _desc new decription.
			 */
			void setDescription(const etk::String& _desc);
			/**
			 * @brief Tag the signal as periodic...
			 * @param[in] _state state of the periodic element
			 */
			void setPeriodic(bool _state);
			/**
			 * @brief Get the number of observers connected on the signal.
			 * @return The count of observer.
			 */
			virtual size_t size() const = 0;
			/**
			 * @brief Check if we have a connected observers.
			 * @return true More than one observers.
			 * @return false No observers.
			 */
			virtual bool empty() const = 0;
			/**
			 * @brief Clear all connectd observers.
			 */
			virtual void clear() = 0;
	};
	/**
	 * @brief Base signal interface for esignal::Signal (permit to create abstract list of signals...)
	 */
	class Base {
		protected:
			ememory::SharedPtr<esignal::BaseInternal> m_data;
		public:
			/**
			 * @brief Basic constructor:
			 */
			Base();
			//! @brief Copy constructor:
			Base(const Base&) = delete;
			//! @brief Move constructor
			Base(Base&&) = delete;
			/**
			 * @brief Virtualize the destructor
			 * @internal
			 */
			virtual ~Base();
			/**
			 * @brief Disconnect the shared_ptr form the Signal
			 * @param[in] _obj Link with the object to check
			 */
			virtual void disconnectShared(const ememory::SharedPtr<void>& _obj) = 0;
			/**
			 * @brief Get name of the signal.
			 * @return requested name.
			 */
			virtual const etk::String& getName() const;
			/**
			 * @brief Get decription of the signal.
			 * @return requested decription.
			 */
			virtual const etk::String& getDescription() const;
			/**
			 * @brief Tag the signal as periodic...
			 * @param[in] _state state of the periodic element
			 */
			void setPeriodic(bool _state);
	};
	//! @not-in-doc
	etk::Stream& operator <<(etk::Stream& _os, const esignal::Base& _obj);
	#ifdef DEBUG
		const char* logIndent(int32_t _iii);
	#endif
}
