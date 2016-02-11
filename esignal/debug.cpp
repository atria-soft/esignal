/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#include <esignal/debug.h>

int32_t esignal::getLogId() {
	static int32_t g_val = etk::log::registerInstance("esignal");
	return g_val;
}
