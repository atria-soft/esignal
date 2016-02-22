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
#define __class__ "test_signal_class_func"
class testCallbackShared : public std::enable_shared_from_this<testCallbackShared> {
	public:
		int32_t m_int32;
		std::string m_string;
		bool m_void;
		testCallbackShared() {
			m_void = false;
			m_int32 = 0;
			m_string = "";
		}
		virtual ~testCallbackShared() {
			
		}
		void callbackVoid() {
			m_void = true;
		}
		void callbackInt(int32_t _a) {
			m_int32 = _a;
		}
		void callbackConstInt(const int32_t& _a) {
			m_int32 = _a;
		}
		void callbackString(std::string _b) {
			m_string = _b;
		}
		void callbackConstString(const std::string& _b) {
			m_string = _b;
		}
		void callbackIntString(int32_t _a, std::string _b) {
			m_int32 = _a;
			m_string = _b;
		}
		void callbackConstIntString(const int32_t& _a, const std::string& _b) {
			m_int32 = _a;
			m_string = _b;
		}
		void callbackMixedIntString(int32_t _a, const std::string& _b) {
			m_int32 = _a;
			m_string = _b;
		}
		void callbackPolyargs(const int32_t& _a, const std::string& _b, char _char, int _int) {
			m_int32 = _a + _int;
			m_string = _b + _char;
		}
		void callbackDisconnect(esignal::Signal<>* _signal) {
			_signal->disconnectShared(shared_from_this());
		}
};


TEST(test_signal_shared_ptr_func, localFunctionVoid) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackVoid);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit();
	EXPECT_EQ(localClass->m_void, true);
}

TEST(test_signal_shared_ptr_func, localFunctionInt32) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	#if 0
		signal.connect(localClass, &testCallbackShared::callbackInt);
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit(12345);
		EXPECT_EQ(localClass->m_int32, 12345);
	#else
		TEST_TODO("can not connect shared_ptr class with other than 'const &' signal parameter ==> NOT BUILD");
	#endif
}

TEST(test_signal_shared_ptr_func, localFunctionConstInt32) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(34567);
	EXPECT_EQ(localClass->m_int32, 34567);
}

TEST(test_signal_shared_ptr_func, localFunctionString) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	#if 0
		signal.connect(localClass, &testCallbackShared::callbackString);
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit("plop");
		EXPECT_EQ(localClass->m_string, "plop");
	#else
		TEST_TODO("can not connect shared_ptr class with other than 'const &' signal parameter ==> NOT BUILD");
	#endif
}

TEST(test_signal_shared_ptr_func, localFunctionConstString) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackConstString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop1234");
	EXPECT_EQ(localClass->m_string, "plop1234");
}

TEST(test_signal_shared_ptr_func, localFunctionIntString) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	#if 0
		signal.connect(localClass, &testCallbackShared::callbackIntString);
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit(23456, "plop2");
		EXPECT_EQ(localClass->m_int32, 23456);
		EXPECT_EQ(localClass->m_string, "plop2");
	#else
		TEST_TODO("can not connect shared_ptr class with other than 'const &' signal parameter ==> NOT BUILD");
	#endif
}

TEST(test_signal_shared_ptr_func, localFunctionConstIntString) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackConstIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(localClass->m_int32, 246);
	EXPECT_EQ(localClass->m_string, "plop567");
}

TEST(test_signal_shared_ptr_func, localFunctionMixedIntString) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	#if 0
		signal.connect(localClass, &testCallbackShared::callbackMixedIntString);
		EXPECT_EQ(signal.size(), 1);
		EXPECT_EQ(signal.empty(), false);
		signal.emit(765, "plTGY");
		EXPECT_EQ(localClass->m_int32, 765);
		EXPECT_EQ(localClass->m_string, "plTGY");
	#else
		TEST_TODO("can not connect shared_ptr class with other than 'const &' signal parameter ==> NOT BUILD");
	#endif
}


TEST(test_signal_shared_ptr_func, localFunctionConstIntStringPolyArg) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackPolyargs, 'c', 12365);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(localClass->m_int32, 246 + 12365);
	EXPECT_EQ(localClass->m_string, "plop567c");
}

class testCallbackSharedHerited : public testCallbackShared {
	public:
		void callbackHerited(const int32_t& _a, const std::string& _b) {
			m_int32 = _a;
			m_string = _b;
		}
};


TEST(test_signal_shared_ptr_func, localFunctionHerited) {
	std::shared_ptr<testCallbackSharedHerited> localClass = std::make_shared<testCallbackSharedHerited>();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackSharedHerited::callbackHerited);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(localClass->m_int32, 246);
	EXPECT_EQ(localClass->m_string, "plop567");
}


TEST(test_signal_shared_ptr_func, disconnect) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(34567);
	EXPECT_EQ(localClass->m_int32, 34567);
	signal.disconnectShared(localClass);
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}


TEST(test_signal_shared_ptr_func, connect_disconnect_multiple) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<int32_t> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 2);
	EXPECT_EQ(signal.empty(), false);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 3);
	EXPECT_EQ(signal.empty(), false);
	signal.connect(localClass, &testCallbackShared::callbackConstInt);
	EXPECT_EQ(signal.size(), 4);
	EXPECT_EQ(signal.empty(), false);
	signal.disconnectShared(localClass);
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.disconnectShared(localClass);
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}


TEST(test_signal_shared_ptr_func, disconnect_inCallback) {
	std::shared_ptr<testCallbackShared> localClass = std::make_shared<testCallbackShared>();
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	signal.connect(localClass, &testCallbackShared::callbackDisconnect, &signal);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit();
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
}

