/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#define NAME "Empty"

#include <etk/types.hpp>
#include <etk/etk.hpp>
#include <test-debug/debug.hpp>
#include <etest/etest.hpp>

int main(int _argc, const char *_argv[]) {
	etest::init(_argc, _argv);
	for (int32_t iii=0; iii<_argc ; ++iii) {
		etk::String data = _argv[iii];
		if (    data == "-h"
		     || data == "--help") {
			TEST_PRINT("esignal-test - help : ");
			TEST_PRINT("    " << _argv[0] << " [options]");
			TEST_PRINT("        No optiions ...");
			return -1;
		}
	}
	return RUN_ALL_TESTS();
}
