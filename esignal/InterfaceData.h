/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <map>

namespace esignal {
	class Base;
	/**
	 * @brief Interface data to collect the Signal list (for abstarction connection)
	 */
	class InterfaceData {
		private:
			std::vector<esignal::Base*> m_list;  //!< list of availlable Parameters
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
			std::vector<std::string> getAll() const;
			/**
			 * @brief Remove binding on all event class.
			 * @param[in] _sharedPtr sharedPtr to unlink (no type needed ...).
			 */
			void disconnect(const std::shared_ptr<void>& _sharedPtr);
	};
}

