/**
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
#include <memory>
#include <test-debug/debug.h>


#undef __class__
#define __class__ "test_signal_arg"
/*
class TestSingleArgs : public esignal::Interface {
	public:
		esignal::Signal<int32_t, int32_t> signalInt32;
		
		TestSingleArgs() :
		  signalInt32(*this, "value-int", "Select value change integer") {
			
		}
		void emit() {
			TEST_DEBUG("Emit signal int32 : value=22,23");
			signalInt32.emit(22, 23);
		}
};

class TestConnect : public std::enable_shared_from_this<TestConnect> {
	public:
		int32_t m_valueInt32_1;
		int32_t m_valueInt32_2;
	public:
		TestConnect():
		  m_valueInt32_1(0),
		  m_valueInt32_2(0) {
			
		}
		void connect(TestSingleArgs& _remote) {
			TEST_DEBUG("connect on signalInt32");
			_remote.signalInt32.bind(shared_from_this(), &TestConnect::onCallbackInt);
		}
		void onCallbackInt(const int32_t& _value1, const int32_t& _value2) {
			TEST_DEBUG("onCallbackInt " << _value1 << " " << _value2);
			m_valueInt32_1 = _value1;
			m_valueInt32_2 = _value2;
		}
};


TEST(test_signal_arg, checkType) {
	TestSingleArgs baseClass;
	std::shared_ptr<TestConnect> connectedClass = std::make_shared<TestConnect>();
	connectedClass->connect(baseClass);
	baseClass.emit();
	EXPECT_EQ(connectedClass->m_valueInt32_1, 22);
	EXPECT_EQ(connectedClass->m_valueInt32_2, 23);
}

*/