/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esignal/debug.hpp>

int32_t esignal::getLogId() {
	static int32_t g_val = elog::registerInstance("esignal");
	return g_val;
}
