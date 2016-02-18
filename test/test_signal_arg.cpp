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
class TestSingleArg : public esignal::Interface {
	public:
		esignal::Signal<int32_t> signalInt32;
		
		TestSingleArg() :
		  signalInt32(*this, "value-int", "Select value change integer") {
			
		}
		void emit() {
			TEST_DEBUG("Emit signal int32 : value=22");
			signalInt32.emit(22);
		}
};

class TestConnect : public std::enable_shared_from_this<TestConnect> {
	public:
		int32_t m_valueInt32;
	public:
		TestConnect():
		  m_valueInt32(0) {
			
		}
		void connect(TestSingleArg& _remote) {
			TEST_DEBUG("connect on signalInt32");
			_remote.signalInt32.bind(shared_from_this(), &TestConnect::onCallbackInt);
		}
		void onCallbackInt(const int32_t& _value) {
			TEST_DEBUG("onCallbackInt " << _value);
			m_valueInt32 = _value;
		}
};
*/

class TestConnect {
	public:
		TestConnect() {
			
		}
		void display_values_4( int a, float b, char c) {
			std::cout << " 4444 " << a << " " << b << " " << c << std::endl;
		}
		void display_values_5( int a, float b, char c, std::string _plopppp) {
			std::cout << " 5555 " << a << " " << b << " " << c << " " << _plopppp << std::endl;
		}
};

class TestConnectShared : public std::enable_shared_from_this<TestConnectShared>{
	public:
		TestConnectShared() {
			
		}
		void display_values_6( int a, float b, char c) {
			std::cout << " 6666 " << a << " " << b << " " << c << std::endl;
		}
		void display_values_7( int a, float b, char c, std::string _plopppp) {
			std::cout << " 7777 " << a << " " << b << " " << c << " " << _plopppp << std::endl;
		}
};

template<class Func, class... Arg>
auto complete_args(Func _f, Arg... _arg) {
	return [=]( auto&&... cargs ){ return _f( cargs..., _arg... ); };
}


template<class pointer, class Func, class... Arg>
auto complete_class(pointer* _class, Func _f, Arg... _arg) {
	return [=]( auto&&... cargs ){
		//_class->*_f(cargs..., _arg...);
		(*_class.*_f)(cargs..., _arg...);
	};
}

TEST(test_signal_arg, checkType) {
	/*
	TestSingleArg baseClass;
	std::shared_ptr<TestConnect> connectedClass = std::make_shared<TestConnect>();
	connectedClass->connect(baseClass);
	baseClass.emit();
	EXPECT_EQ(connectedClass->m_valueInt32, 22);
	*/
	esignal::Signal<int, float, char> signal;
	// ----------------------------------------------------
	auto display_values_1 = []( int a, float b, char c){
		std::cout << " 1111 " << a << " " << b << " " << c << " " << std::endl;
	};
	signal.connect(display_values_1);
	
	// ----------------------------------------------------
	auto display_values_2 = []( int a, float b, char c, int w ){
		std::cout << " 2222 " << a << " " << b << " " << c << " " << w << std::endl;
	};
	signal.connect( complete_args( display_values_2, 72 ) );
	
	// ----------------------------------------------------
	TestConnect connectedClass;
	signal.connect( [&](int a, float b, char c) {
	                	connectedClass.display_values_4(a,b,c);}
	                );
	/*
	signal.connect( [&](auto && ... _cargs) {
	                	connectedClass.display_values_3(_cargs);}
	                );
	*/
	signal.connect(&connectedClass, &TestConnect::display_values_4);
	signal.connect(&connectedClass, &TestConnect::display_values_5, "coucou");
	
	std::shared_ptr<TestConnectShared> connectedClassShared = std::make_shared<TestConnectShared>();
	signal.connect(connectedClassShared, &TestConnectShared::display_values_6);
	signal.connect(connectedClassShared, &TestConnectShared::display_values_7, "coucou");
	
	
	//signal.connect( complete_class(&connectedClass, &TestConnect::display_values_3) );
	//signal.connect( TestConnect::display_values_3(&connectedClass) );
	
	
	// ----------------------------------------------------
	
	signal.emit( 42, 2.99, 'k');
	
}
