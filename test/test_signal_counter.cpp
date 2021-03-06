/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */

#define NAME "SingleArg"
#include <etest/etest.hpp>
#include <esignal/Signal.hpp>
#include <esignal/Interface.hpp>
#include <ememory/memory.hpp>
#include <test-debug/debug.hpp>

class testCounter {
	public:
		esignal::Signal<etk::String> m_signal;
	public:
		size_t m_int32;
		testCounter():
		  m_signal(this, &testCounter::changeCount) {
			m_int32 = 0;
		}
		void changeCount(size_t _a){
			TEST_VERBOSE("connection number : " << _a);
			m_int32 = _a;
		}
};

TEST(test_signal_counter, localbasicCounter) {
	testCounter localClass;
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
	esignal::Connection connection1 = localClass.m_signal.connect([=](etk::String){});
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	esignal::Connection connection2 = localClass.m_signal.connect([=](etk::String){});
	EXPECT_EQ(localClass.m_signal.size(), 2);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 2);
	connection1.disconnect();
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	connection2.disconnect();
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
}


TEST(test_signal_counter, localbasicCopy) {
	testCounter localClass;
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
	esignal::Connection connection1 = localClass.m_signal.connect([=](etk::String){});
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	esignal::Connection connection2 = etk::move(connection1);
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	connection1.disconnect();
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	connection2.disconnect();
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
}


TEST(test_signal_counter, localbasicCheckConnection) {
	testCounter localClass;
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
	esignal::Connection connection1 = localClass.m_signal.connect([=](etk::String){});
	EXPECT_EQ(connection1.isConnected(), true);
	connection1.disconnect();
	EXPECT_EQ(connection1.isConnected(), false);
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
}
