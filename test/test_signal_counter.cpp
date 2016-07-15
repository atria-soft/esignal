/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#define NAME "SingleArg"
#include <gtest/gtest.h>
#include <esignal/Signal.h>
#include <esignal/Interface.h>
#include <ememory/memory.h>
#include <test-debug/debug.h>

class testCounter {
	public:
		esignal::Signal<std::string> m_signal;
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
	esignal::Connection connection1 = localClass.m_signal.connect([=](std::string){});
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	esignal::Connection connection2 = localClass.m_signal.connect([=](std::string){});
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
	esignal::Connection connection1 = localClass.m_signal.connect([=](std::string){});
	EXPECT_EQ(localClass.m_signal.size(), 1);
	EXPECT_EQ(localClass.m_signal.empty(), false);
	EXPECT_EQ(localClass.m_int32, 1);
	esignal::Connection connection2 = std::move(connection1);
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
	esignal::Connection connection1 = localClass.m_signal.connect([=](std::string){});
	EXPECT_EQ(connection1.isConnected(), true);
	connection1.disconnect();
	EXPECT_EQ(connection1.isConnected(), false);
	EXPECT_EQ(localClass.m_signal.size(), 0);
	EXPECT_EQ(localClass.m_signal.empty(), true);
	EXPECT_EQ(localClass.m_int32, 0);
}
