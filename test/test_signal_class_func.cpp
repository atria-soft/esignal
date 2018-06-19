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

class testCallback {
	public:
		int32_t m_int32;
		etk::String m_string;
		bool m_void;
		testCallback() {
			m_emptyFunctor = null;
			m_void = false;
			m_int32 = 0;
			m_string = "";
		}
		using stupidFunctor = etk::Function<void()>;
		
		stupidFunctor m_emptyFunctor;
		
		void callbackVoid(){
			TEST_VERBOSE("event void");
			m_void = true;
		}
		void callbackInt(int32_t _a){
			TEST_VERBOSE("event a=" << _a);
			m_int32 = _a;
		}
		void callbackConstInt(const int32_t& _a){
			TEST_VERBOSE("event a=" << _a);
			m_int32 = _a;
		}
		void callbackString(etk::String _b){
			TEST_VERBOSE("event b=" << _b);
			m_string = _b;
		}
		void callbackConstString(const etk::String& _b){
			TEST_VERBOSE("event b=" << _b);
			m_string = _b;
		}
		void callbackIntString(int32_t _a, etk::String _b){
			TEST_VERBOSE("event a=" << _a << " b=" << _b);
			m_int32 = _a;
			m_string = _b;
		}
		void callbackConstIntString(const int32_t& _a, const etk::String& _b){
			TEST_VERBOSE("event a=" << _a << " b=" << _b);
			m_int32 = _a;
			m_string = _b;
		}
		void callbackMixedIntString(int32_t _a, const etk::String& _b){
			TEST_VERBOSE("event a=" << _a << " b=" << _b);
			m_int32 = _a;
			m_string = _b;
		}
		void callbackPolyargs(const int32_t& _a, const etk::String& _b, char _char, int _int) {
			TEST_VERBOSE("event a=" << _a << " b=" << _b << " _char=" << _char << " _int=" << _int);
			m_int32 = _a + _int;
			m_string = _b + _char;
		}
		void callbackDisconnect(esignal::Connection* _connection) {
			TEST_VERBOSE("Disconnect ...");
			_connection->disconnect();
		}
};

TEST(test_signal_class_func, localLog) {
	esignal::Signal<> signal;
	TEST_INFO("dtrem log : " << signal);
}

TEST(test_signal_class_func, force_clear_all_connection) {
	testCallback localClass;
	esignal::Signal<> signal;
	signal.clear();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackVoid);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.clear();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	connection1.disconnect();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.emit();
	EXPECT_EQ(localClass.m_void, false);
}

TEST(test_signal_class_func, localbasicNameDesc) {
	esignal::Signal<> signal;
	EXPECT_EQ(signal.getName(), "");
	EXPECT_EQ(signal.getDescription(), "");
}

TEST(test_signal_class_func, localNullClass) {
	testCallback* localClass = null;
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(localClass, &testCallback::callbackVoid);
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.emit();
}

/*
BAD case ...
TEST(test_signal_class_func, localNullFunc) {
	testCallback localClass;
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::m_emptyFunctor);
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.emit();
	EXPECT_EQ(localClass.m_void, false);
}
*/

TEST(test_signal_class_func, localFunctionVoid) {
	testCallback localClass;
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackVoid);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit();
	EXPECT_EQ(localClass.m_void, true);
}

TEST(test_signal_class_func, localFunctionInt32) {
	testCallback localClass;
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(12345);
	EXPECT_EQ(localClass.m_int32, 12345);
}

TEST(test_signal_class_func, localFunctionConstInt32) {
	testCallback localClass;
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(34567);
	EXPECT_EQ(localClass.m_int32, 34567);
}

TEST(test_signal_class_func, localFunctionString) {
	testCallback localClass;
	esignal::Signal<etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop");
	EXPECT_EQ(localClass.m_string, "plop");
}

TEST(test_signal_class_func, localFunctionConstString) {
	testCallback localClass;
	esignal::Signal<etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackConstString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop1234");
	EXPECT_EQ(localClass.m_string, "plop1234");
}

TEST(test_signal_class_func, localFunctionIntString) {
	testCallback localClass;
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(23456, "plop2");
	EXPECT_EQ(localClass.m_int32, 23456);
	EXPECT_EQ(localClass.m_string, "plop2");
}

TEST(test_signal_class_func, localFunctionConstIntString) {
	testCallback localClass;
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackConstIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(localClass.m_int32, 246);
	EXPECT_EQ(localClass.m_string, "plop567");
}

TEST(test_signal_class_func, localFunctionMixedIntString) {
	testCallback localClass;
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackMixedIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(765, "plTGY");
	EXPECT_EQ(localClass.m_int32, 765);
	EXPECT_EQ(localClass.m_string, "plTGY");
}

TEST(test_signal_class_func, localFunctionConstIntStringPolyArg) {
	testCallback localClass;
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackPolyargs, 'c', 12365);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(localClass.m_int32, 246 + 12365);
	EXPECT_EQ(localClass.m_string, "plop567c");
}


TEST(test_signal_class_func, disconnect) {
	testCallback localClass;
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(12345);
	EXPECT_EQ(localClass.m_int32, 12345);
	connection1.disconnect();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}


TEST(test_signal_class_func, connect_disconnect_multiple) {
	testCallback localClass;
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection2 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 2);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection3 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 3);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection4 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 4);
	EXPECT_EQ(signal.empty(), false);
	connection1.disconnect();
	EXPECT_EQ(signal.size(), 3);
	EXPECT_EQ(signal.empty(), false);
	connection2.disconnect();
	EXPECT_EQ(signal.size(), 2);
	EXPECT_EQ(signal.empty(), false);
	connection3.disconnect();
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	connection4.disconnect();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	connection1.disconnect();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection5;
	connection5.disconnect();
	connection5 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	connection5 = signal.connect(&localClass, &testCallback::callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
}


TEST(test_signal_class_func, disconnect_inCallback) {
	testCallback localClass;
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1;
	connection1 = signal.connect(&localClass, &testCallback::callbackDisconnect, &connection1);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}

