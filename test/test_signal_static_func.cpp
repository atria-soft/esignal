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

static int32_t tmpRetInt32 = 0;
static etk::String tmpRetString = "";
static bool tmpRetVoid = false;

static void clear() {
	tmpRetVoid = false;
	tmpRetInt32 = 0;
	tmpRetString = "";
}

static void callbackVoid(){
	TEST_VERBOSE("event void");
	tmpRetVoid = true;
};

static void callbackInt(int32_t _a){
	TEST_VERBOSE("event a=" << _a);
	tmpRetInt32 = _a;
};

static void callbackConstInt(const int32_t& _a){
	TEST_VERBOSE("event a=" << _a);
	tmpRetInt32 = _a;
};

static void callbackString(etk::String _b){
	TEST_VERBOSE("event b=" << _b);
	tmpRetString = _b;
};

static void callbackConstString(const etk::String& _b){
	TEST_VERBOSE("event b=" << _b);
	tmpRetString = _b;
};

static void callbackIntString(int32_t _a, etk::String _b){
	TEST_VERBOSE("event a=" << _a << " b=" << _b);
	tmpRetInt32 = _a;
	tmpRetString = _b;
};

static void callbackConstIntString(const int32_t& _a, const etk::String& _b){
	TEST_VERBOSE("event a=" << _a << " b=" << _b);
	tmpRetInt32 = _a;
	tmpRetString = _b;
};

static void callbackMixedIntString(int32_t _a, const etk::String& _b){
	TEST_VERBOSE("event a=" << _a << " b=" << _b);
	tmpRetInt32 = _a;
	tmpRetString = _b;
};

static void callbackPolyargs(const int32_t& _a, const etk::String& _b, char _char, int _int) {
	TEST_VERBOSE("event a=" << _a << " b=" << _b << " _char=" << _char << " _int=" << _int);
	tmpRetInt32 = _a + _int;
	tmpRetString = _b + _char;
}

TEST(test_signal_static_func, localFunctionVoid) {
	clear();
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackVoid);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit();
	EXPECT_EQ(tmpRetVoid, true);
}

TEST(test_signal_static_func, localFunctionInt32) {
	clear();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(12345);
	EXPECT_EQ(tmpRetInt32, 12345);
}

TEST(test_signal_static_func, localFunctionConstInt32) {
	clear();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(34567);
	EXPECT_EQ(tmpRetInt32, 34567);
}

TEST(test_signal_static_func, localFunctionString) {
	clear();
	esignal::Signal<etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop");
	EXPECT_EQ(tmpRetString, "plop");
}

TEST(test_signal_static_func, localFunctionConstString) {
	clear();
	esignal::Signal<etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackConstString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop1234");
	EXPECT_EQ(tmpRetString, "plop1234");
}

TEST(test_signal_static_func, localFunctionIntString) {
	clear();
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(23456, "plop2");
	EXPECT_EQ(tmpRetInt32, 23456);
	EXPECT_EQ(tmpRetString, "plop2");
}

TEST(test_signal_static_func, localFunctionConstIntString) {
	clear();
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackConstIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(tmpRetInt32, 246);
	EXPECT_EQ(tmpRetString, "plop567");
}

TEST(test_signal_static_func, localFunctionMixedIntString) {
	clear();
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackMixedIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(765, "plTGY");
	EXPECT_EQ(tmpRetInt32, 765);
	EXPECT_EQ(tmpRetString, "plTGY");
}

#if __CPP_VERSION__ >= 2014
	template<class Func, class... Arg>
	auto complete_args(Func _f, Arg... _arg) {
		return [=]( auto&&... cargs ){
		    return _f( cargs..., _arg... );
		};
	}
	TEST(test_signal_static_func, localFunctionConstIntStringPolyArg) {
		clear();
		esignal::Signal<int32_t, etk::String> signal;
		EXPECT_EQ(signal.size(), 0);
		EXPECT_EQ(signal.empty(), true);
		esignal::Connection connection1 = signal.connect(complete_args(&callbackPolyargs, 'c', 12365));
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit(246, "plop567");
		EXPECT_EQ(tmpRetInt32, 246 + 12365);
		EXPECT_EQ(tmpRetString, "plop567c");
	}
#else
	/*
	template<class Func, class... ARGS_BASE, class... ARGS_CURR>
	std::function<void(const ARGS_BASE&...)> complete_args(Func _f, ARGS_BASE... _arg) {
		return [=](const ARGS_BASE&..., ARGS_CURR...){
		    return _f( cargs..., _arg... );
		};
	}
	*/
#endif

#if 0
TEST(test_signal_static_func, localFunctionConstIntStringPolyArgWithJAJA) {
	clear();
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackPolyargs, 'c', 12365);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(tmpRetInt32, 246 + 12365);
	EXPECT_EQ(tmpRetString, "plop567c");
}
#endif

TEST(test_signal_static_func, localFunctionLambda) {
	clear();
	esignal::Signal<int32_t, etk::String> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(
	    [](int32_t _a, const etk::String& _b) {
	        tmpRetInt32 = _a;
	        tmpRetString = _b;
	        }
	    );
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(tmpRetInt32, 246);
	EXPECT_EQ(tmpRetString, "plop567");
}

TEST(test_signal_static_func, disconnect) {
	clear();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(12345);
	EXPECT_EQ(tmpRetInt32, 12345);
	connection1.disconnect();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}

TEST(test_signal_static_func, connect_disconnect_multiple) {
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection2 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 2);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection3 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 3);
	EXPECT_EQ(signal.empty(), false);
	esignal::Connection connection4 = signal.connect(&callbackInt);
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
	connection5 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	connection5 = signal.connect(&callbackInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
}

#if __CPP_VERSION__ >= 2014
	static void callbackDisconnect(esignal::Connection* _connection) {
		_connection->disconnect();
	}
	TEST(test_signal_static_func, disconnect_inCallback) {
		esignal::Signal<> signal;
		EXPECT_EQ(signal.size(), 0);
		EXPECT_EQ(signal.empty(), true);
		esignal::Connection connection1;
		connection1 = signal.connect(complete_args(&callbackDisconnect, &connection1));
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit();
		EXPECT_EQ(signal.size(), 0);
		EXPECT_EQ(signal.empty(), true);
	}
#endif

