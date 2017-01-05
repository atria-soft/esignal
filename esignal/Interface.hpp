/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <vector>
#include <map>
#include <esignal/InterfaceData.hpp>

namespace esignal {
	/**
	 * @brief Interface to collect the Signal list (for abstarction connection)
	 * It create a simple "signals" member that permit to access at the signalproperties.
	 */
	class Interface {
		public:
			esignal::InterfaceData signals; //!< Interface to access at all signals properties...
	};
}

