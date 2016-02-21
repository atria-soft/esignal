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
#define __class__ "test_signal_static_func"

static int32_t tmpRetInt32 = 0;
static std::string tmpRetString = "";
static bool tmpRetVoid = false;

static void clear() {
	tmpRetVoid = false;
	tmpRetInt32 = 0;
	tmpRetString = "";
}

auto callbackVoid = [](){
	tmpRetVoid = true;
};

auto callbackInt = [](int32_t _a){
	tmpRetInt32 = _a;
};

auto callbackConstInt = [](const int32_t& _a){
	tmpRetInt32 = _a;
};

auto callbackString = [](std::string _b){
	tmpRetString = _b;
};

auto callbackConstString = [](const std::string& _b){
	tmpRetString = _b;
};

auto callbackIntString = [](int32_t _a, std::string _b){
	tmpRetInt32 = _a;
	tmpRetString = _b;
};

auto callbackConstIntString = [](const int32_t& _a, const std::string& _b){
	tmpRetInt32 = _a;
	tmpRetString = _b;
};

auto callbackMixedIntString = [](int32_t _a, const std::string& _b){
	tmpRetInt32 = _a;
	tmpRetString = _b;
};


TEST(test_signal_static_func, localFunctionVoid) {
	clear();
	esignal::Signal<> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackVoid);
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
	esignal::Connection connection1 = signal.connect(callbackInt);
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
	esignal::Connection connection1 = signal.connect(callbackConstInt);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(34567);
	EXPECT_EQ(tmpRetInt32, 34567);
}

TEST(test_signal_static_func, localFunctionString) {
	clear();
	esignal::Signal<std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop");
	EXPECT_EQ(tmpRetString, "plop");
}

TEST(test_signal_static_func, localFunctionConstString) {
	clear();
	esignal::Signal<std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackConstString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit("plop1234");
	EXPECT_EQ(tmpRetString, "plop1234");
}

TEST(test_signal_static_func, localFunctionIntString) {
	clear();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(23456, "plop2");
	EXPECT_EQ(tmpRetInt32, 23456);
	EXPECT_EQ(tmpRetString, "plop2");
}

TEST(test_signal_static_func, localFunctionConstIntString) {
	clear();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackConstIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(246, "plop567");
	EXPECT_EQ(tmpRetInt32, 246);
	EXPECT_EQ(tmpRetString, "plop567");
}

TEST(test_signal_static_func, localFunctionMixedIntString) {
	clear();
	esignal::Signal<int32_t, std::string> signal;
	EXPECT_EQ(signal.size(), 0);
	EXPECT_EQ(signal.empty(), true);
	esignal::Connection connection1 = signal.connect(callbackMixedIntString);
	EXPECT_EQ(signal.size(), 1);
	EXPECT_EQ(signal.empty(), false);
	signal.emit(765, "plTGY");
	EXPECT_EQ(tmpRetInt32, 765);
	EXPECT_EQ(tmpRetString, "plTGY");
}

#if 0
	// ----------------------------------------------------
	auto display_values_2 = []( int a, float b, char c, int w ){
		std::cout << " 2222 " << a << " " << b << " " << c << " " << w << std::endl;
	};
	esignal::Connection h2 = signal.connect( complete_args( display_values_2, 72 ) );
	
	// ----------------------------------------------------
	TestConnect connectedClass;
	esignal::Connection h3 = signal.bind([&](int a, float b, char c) {
	                                        	connectedClass.display_values_4(a,b,c);
	                                        });
	/*
	signal.bind( [&](auto && ... _cargs) {
	                	connectedClass.display_values_3(_cargs);}
	                );
	*/
	esignal::Connection h4 = signal.bind(&connectedClass, &TestConnect::display_values_4);
	esignal::Connection h5 = signal.bind(&connectedClass, &TestConnect::display_values_5, "coucou");
	
	std::shared_ptr<TestConnectShared> connectedClassShared = std::make_shared<TestConnectShared>();
	signal.bind(connectedClassShared, &TestConnectShared::display_values_6);
	signal.bind(connectedClassShared, &TestConnectShared::display_values_7, "coucou");
	
	
	signal.emit( 5, 2.99, 'k');
}

TEST(test_signal_static_func, localFunction) {
	
	esignal::Signal<int, float, char> signal;
	// ----------------------------------------------------
	auto display_values_1 = []( int a, float b, char c){
		std::cout << " 1111 " << a << " " << b << " " << c << " " << std::endl;
	};
	esignal::Connection h1 = signal.bind(display_values_1);
	
	// ----------------------------------------------------
	auto display_values_2 = []( int a, float b, char c, int w ){
		std::cout << " 2222 " << a << " " << b << " " << c << " " << w << std::endl;
	};
	esignal::Connection h2 = signal.bind( complete_args( display_values_2, 72 ) );
	
	// ----------------------------------------------------
	TestConnect connectedClass;
	esignal::Connection h3 = signal.bind([&](int a, float b, char c) {
	                                        	connectedClass.display_values_4(a,b,c);
	                                        });
	/*
	signal.bind( [&](auto && ... _cargs) {
	                	connectedClass.display_values_3(_cargs);}
	                );
	*/
	esignal::Connection h4 = signal.bind(&connectedClass, &TestConnect::display_values_4);
	esignal::Connection h5 = signal.bind(&connectedClass, &TestConnect::display_values_5, "coucou");
	
	std::shared_ptr<TestConnectShared> connectedClassShared = std::make_shared<TestConnectShared>();
	signal.bind(connectedClassShared, &TestConnectShared::display_values_6);
	signal.bind(connectedClassShared, &TestConnectShared::display_values_7, "coucou");
	
	
	signal.emit( 5, 2.99, 'k');
}

#endif


