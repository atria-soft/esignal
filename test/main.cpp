/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#define NAME "Empty"

#include <etk/types.h>
#include <etk/etk.h>
#include <test-debug/debug.h>
#include <gtest/gtest.h>

int main(int _argc, const char *_argv[]) {
	::testing::InitGoogleTest(&_argc, const_cast<char **>(_argv));
	etk::init(_argc, _argv);
	for (int32_t iii=0; iii<_argc ; ++iii) {
		std::string data = _argv[iii];
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
