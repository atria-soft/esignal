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
	class Interface {
		friend class esignal::Base; // to register parameter in the list.
		private:
			std::vector<esignal::Base*> m_list;  //!< list of availlable Parameters
		public:
			/**
			 * @brief Constructor.
			 */
			Interface();
			/**
			 * @brief Destructor.
			 */
			virtual ~Interface();
			/**
			 * @brief Register a parameter class pointer in the List of parameters
			 * @note This class does not destroy the parameter pointer!!!
			 * @param[in] _pointerOnSignal Pointer on the signal that might be added.
			 */
			void signalAdd(esignal::Base* _pointerOnSignal);
			void signalRemove(esignal::Base* _pointerOnSignal);
			/**
			 * @brief Get All the signal list:
			 * @return vector on all the signals names
			 */
			std::vector<std::string> signalGetAll() const;
			/**
			 * @brief Remove binding on all event class.
			 * @param[in] _sharedPtr sharedPtr to unlink (no type needed ...).
			 */
			void signalDisconnect(const std::shared_ptr<void>& _sharedPtr);
	};
}

