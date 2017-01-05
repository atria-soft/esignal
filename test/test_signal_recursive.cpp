/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#define NAME "SingleArg"
#include <gtest/gtest.h>
#include <esignal/Signal.hpp>
#include <esignal/Interface.hpp>
#include <ememory/memory.hpp>
#include <test-debug/debug.hpp>

static esignal::Signal<int32_t>* signalll;
static int32_t tmpRetInt32 = 0;

static auto callbackInt = [](int32_t _a){
	tmpRetInt32 = _a;
	TEST_VERBOSE("event a=" << _a);
	if (tmpRetInt32 != 0) {
		TEST_VERBOSE("emit a=" << _a-1);
		signalll->emit(_a - 1);
	}
};


TEST(test_signal_recursive, base) {
	tmpRetInt32 = -1;
	signalll = new esignal::Signal<int32_t>();
	EXPECT_EQ(signalll->size(), 0);
	EXPECT_EQ(signalll->empty(), true);
	esignal::Connection connection1 = signalll->connect(callbackInt);
	EXPECT_EQ(signalll->size(), 1);
	EXPECT_EQ(signalll->empty(), false);
	signalll->emit(22);
	EXPECT_EQ(tmpRetInt32, 0);
}

