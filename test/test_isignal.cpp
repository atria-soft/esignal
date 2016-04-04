/**
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#define NAME "SingleArg"
#include <gtest/gtest.h>
#include <esignal/ISignal.h>
#include <esignal/Interface.h>
#include <memory>
#include <test-debug/debug.h>


#undef __class__
#define __class__ "test_isignal"

class testISignal : public esignal::Interface {
	public:
		esignal::ISignal<int32_t> m_signalInt;
		esignal::ISignal<std::string> m_signalString;
		esignal::ISignal<float> m_signalFloat;
	public:
		size_t m_count;
		testISignal():
		  m_signalInt(this, &testISignal::changeCount, "int", "desc int"),
		  m_signalString(this, "string", "desc string"),
		  m_signalFloat(this, "float", "desc float") {
			m_count = 0;
		}
		void changeCount(size_t _a) {
			TEST_VERBOSE("connection number : " << _a);
			m_count = _a;
		}
};

TEST(test_isignal_counter, localbasicCounter1) {
	testISignal localClass;
	EXPECT_EQ(localClass.m_signalString.size(), 0);
	EXPECT_EQ(localClass.m_signalString.empty(), true);
	esignal::Connection connection1 = localClass.m_signalString.connect([=](std::string){});
	EXPECT_EQ(localClass.m_signalString.size(), 1);
	EXPECT_EQ(localClass.m_signalString.empty(), false);
	esignal::Connection connection2 = localClass.m_signalString.connect([=](std::string){});
	EXPECT_EQ(localClass.m_signalString.size(), 2);
	EXPECT_EQ(localClass.m_signalString.empty(), false);
	connection1.disconnect();
	EXPECT_EQ(localClass.m_signalString.size(), 1);
	EXPECT_EQ(localClass.m_signalString.empty(), false);
	connection2.disconnect();
	EXPECT_EQ(localClass.m_signalString.size(), 0);
	EXPECT_EQ(localClass.m_signalString.empty(), true);
}

TEST(test_isignal_counter, localbasicCounter2) {
	testISignal localClass;
	EXPECT_EQ(localClass.m_signalInt.size(), 0);
	EXPECT_EQ(localClass.m_signalInt.empty(), true);
	EXPECT_EQ(localClass.m_count, 0);
	esignal::Connection connection1 = localClass.m_signalInt.connect([=](int32_t){});
	EXPECT_EQ(localClass.m_signalInt.size(), 1);
	EXPECT_EQ(localClass.m_signalInt.empty(), false);
	EXPECT_EQ(localClass.m_count, 1);
	esignal::Connection connection2 = localClass.m_signalInt.connect([=](int32_t){});
	EXPECT_EQ(localClass.m_signalInt.size(), 2);
	EXPECT_EQ(localClass.m_signalInt.empty(), false);
	EXPECT_EQ(localClass.m_count, 2);
	connection1.disconnect();
	EXPECT_EQ(localClass.m_signalInt.size(), 1);
	EXPECT_EQ(localClass.m_signalInt.empty(), false);
	EXPECT_EQ(localClass.m_count, 1);
	connection2.disconnect();
	EXPECT_EQ(localClass.m_signalInt.size(), 0);
	EXPECT_EQ(localClass.m_signalInt.empty(), true);
	EXPECT_EQ(localClass.m_count, 0);
}


TEST(test_isignal_counter, localbasicNameDesc) {
	testISignal localClass;
	EXPECT_EQ(localClass.m_signalFloat.getName(), "float");
	EXPECT_EQ(localClass.m_signalInt.getName(), "int");
	EXPECT_EQ(localClass.m_signalString.getName(), "string");
	EXPECT_EQ(localClass.m_signalFloat.getDescription(), "desc float");
	EXPECT_EQ(localClass.m_signalInt.getDescription(), "desc int");
	EXPECT_EQ(localClass.m_signalString.getDescription(), "desc string");
}

class testCallbackIShared : public std::enable_shared_from_this<testCallbackIShared> {
	public:
		testCallbackIShared() {
		}
		void callbackConstInt(const int32_t& _a) {
			TEST_VERBOSE("event a=" << _a);
		}
};

TEST(test_isignal_counter, localbasicInterfaceGetListSignal) {
	testISignal localClass;
	std::vector<std::string> list;
	list.push_back("int");
	list.push_back("string");
	list.push_back("float");
	EXPECT_EQ(localClass.signal.getAll(), list);
}

TEST(test_isignal_counter, localbasicInterfaceDisconnectNullPtr) {
	testISignal localClass;
	localClass.signal.disconnect(nullptr);
}

TEST(test_isignal_counter, localbasicInterfaceDisconnectSharedPtr) {
	testISignal localClass;
	std::shared_ptr<testCallbackIShared> tmp = std::make_shared<testCallbackIShared>();
	localClass.signal.disconnect(tmp);
	EXPECT_EQ(localClass.m_signalInt.size(), 0);
	EXPECT_EQ(localClass.m_signalInt.empty(), true);
	localClass.m_signalInt.connect(tmp, &testCallbackIShared::callbackConstInt);
	EXPECT_EQ(localClass.m_signalInt.size(), 1);
	EXPECT_EQ(localClass.m_signalInt.empty(), false);
	localClass.m_signalInt.emit(34567);
	localClass.signal.disconnect(tmp);
	EXPECT_EQ(localClass.m_signalInt.size(), 0);
	EXPECT_EQ(localClass.m_signalInt.empty(), true);
}
