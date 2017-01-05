/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2016, Edouard DUPIN, all right reserved
 * 
 * @license MPL v2.0 (see license file)
 */
#include <etk/etk.hpp>
#include <test-debug/debug.hpp>

//! [esignal_sample_declare]
#include <esignal/Signal.hpp>
//! [esignal_sample_declare]

void declareSignals() {
	//! [esignal_sample_declare_int]
	esignal::Signal<int32_t> signalInt;
	//! [esignal_sample_declare_int]
	
	//! [esignal_sample_declare_void]
	esignal::Signal<> signalVoid;
	//! [esignal_sample_declare_void]
	
	//! [esignal_sample_declare_string]
	esignal::Signal<std::string> signalString;
	//! [esignal_sample_declare_string]
}

//! [esignal_sample_connection_simple]
//! [esignal_sample_connection_simple_extern_function]
void localCallBack(int32_t _val) {
	TEST_PRINT("Callback Local the value is : " << _val);
}
//! [esignal_sample_connection_simple_extern_function]

void basicConnection() {
	// Create the signal
	//! [esignal_sample_connection_simple_connection_declare]
	esignal::Signal<int32_t> signalValue;
	//! [esignal_sample_connection_simple_connection_declare]
	// Connect the signal function
	//! [esignal_sample_connection_simple_connection_function]
	esignal::Connection con1 = signalValue.connect(&localCallBack);
	//! [esignal_sample_connection_simple_connection_function]
	// Connect the signal Lambda
	//! [esignal_sample_connection_simple_connection_lambda]
	esignal::Connection con2 = signalValue.connect(
	    [](int32_t _val) {
	    	TEST_PRINT("Callback 1 the value is : " << _val);
	    });
	//! [esignal_sample_connection_simple_connection_lambda]
	// Emit the signal
	//! [esignal_sample_connection_simple_emit]
	signalValue.emit(1001001);
	//! [esignal_sample_connection_simple_emit]
	// Disconnect the connection n°1
	//! [esignal_sample_connection_simple_disconnect]
	con1.disconnect();
	//! [esignal_sample_connection_simple_disconnect]
	// Second emit to check disconnection
	signalValue.emit(2002002);
}
//! [esignal_sample_connection_simple]

//*********************************************************************************************

//! [esignal_sample_class_connection]
//! [esignal_sample_class_connection_callback]
class TmpClass {
	public:
		void localCallBack(const int32_t& _val) {
			TEST_PRINT("Callback Local the value is : " << _val);
		}
		void localCallBackSecond(const int32_t& _val, const std::string& _otherValue) {
			TEST_PRINT("Callback 2 Local the value is : " << _val << " with perso: '" << _otherValue << "'");
		}
};
//! [esignal_sample_class_connection_callback]

void classConnection() {
	// Create the signal
	esignal::Signal<int32_t> signalValue;
	// Declare the class
	TmpClass myClass;
	// Connect signals
	//! [esignal_sample_class_connection_callback_connect]
	esignal::Connection con1 = signalValue.connect(&myClass, &TmpClass::localCallBack);
	esignal::Connection con2 = signalValue.connect(&myClass, &TmpClass::localCallBackSecond, "Hello, HowAreYou");
	//! [esignal_sample_class_connection_callback_connect]
	// Emit sample signals
	signalValue.emit(4004004);
}

//! [esignal_sample_class_connection]


//! [esignal_sample_shared_connection]

void sharedConnection() {
	// Create the signal
	esignal::Signal<int32_t> signalValue;
	// Declare the class
	ememory::SharedPtr<TmpClass> myClassShared = ememory::makeShared<TmpClass>();
	// Connect signals
	signalValue.connect(myClassShared, &TmpClass::localCallBack);
	// Emit sample signals
	signalValue.emit(7007007);
}
//! [esignal_sample_shared_connection]


//! [esignal_sample_new]

void newSignal() {
	// Declare new signal
	//! [esignal_sample_new_declare]
	esignal::Signal<int32_t, std::string> signalCustum;
	//! [esignal_sample_new_declare]
	// Connect a lambda
	//! [esignal_sample_new_lambda]
	esignal::Connection con2 = signalCustum.connect(
	    [](int32_t _val, std::string _val2) {
	    	TEST_PRINT("lambda callback: " << _val << " vel2=" << _val2);
	    });
	//! [esignal_sample_new_lambda]
	// Example emit
	//! [esignal_sample_new_emit]
	signalCustum.emit(1001001, "plop");
	//! [esignal_sample_new_emit]
}

// do it in a single C++: Implementation of signal
//! [esignal_sample_new_register]
#include <esignal/details/Signal.hxx>
ESIGNAL_DECLARE_SIGNAL(int32_t, std::string);
//! [esignal_sample_new_register]

//! [esignal_sample_new]


int main(int _argc, const char *_argv[]) {
	etk::init(_argc, _argv);
	TEST_INFO("declare [START] ***************************");
	declareSignals();
	TEST_INFO("declare [STOP]  ***************************");
	TEST_INFO("Basic connection [START] ***************************");
	basicConnection();
	TEST_INFO("Basic connection [STOP] ***************************");
	TEST_INFO("class connection [START] ***************************");
	classConnection();
	TEST_INFO("class connection [STOP] ***************************");
	TEST_INFO("shared_ptr connection [START] ***************************");
	sharedConnection();
	TEST_INFO("shared_ptr connection [STOP] ***************************");
	return 0;
}
