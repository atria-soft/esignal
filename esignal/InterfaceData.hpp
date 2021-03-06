/** @file
 * @author Edouard DUPIN
 * @copyright 2016, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/Vector.hpp>
#include <etk/Map.hpp>

namespace esignal {
	class Base;
	/**
	 * @brief Interface data to collect the Signal list (for abstarction connection)
	 */
	class InterfaceData {
		private:
			etk::Vector<esignal::Base*> m_list;  //!< list of availlable Parameters
		public:
			/**
			 * @brief Constructor.
			 */
			InterfaceData();
			/**
			 * @brief Destructor.
			 */
			virtual ~InterfaceData();
			/**
			 * @brief Register a signal class pointer in the List of signal list
			 * @note This class does not destroy the parameter pointer!!!
			 * @param[in] _pointerOnSignal Pointer on the signal that might be added.
			 */
			void add(esignal::Base* _pointerOnSignal);
			/**
			 * @brief Remove a signal class pointer in the List of signal list
			 * @note This class does not destroy the parameter pointer!!!
			 * @param[in] _pointerOnSignal Pointer on the signal that might be removed.
			 */
			void remove(esignal::Base* _pointerOnSignal);
			/**
			 * @brief Get All the signal list:
			 * @return vector on all the signals names
			 */
			etk::Vector<etk::String> getAll() const;
			/**
			 * @brief Remove binding on all event class.
			 * @param[in] _sharedPtr sharedPtr to unlink (no type needed ...).
			 */
			void disconnect(const ememory::SharedPtr<void>& _sharedPtr);
	};
}

