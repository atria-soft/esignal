Tutorial                                         {#esignal_tutorial}
========

@tableofcontents

Declare a Signal:                                {#esignal_declare}
=================

Declaring a signal is really simple, just include the esignal file:

@snippet sampleAll.cpp esignal_sample_declare

You can now declare your signals. We have basicly declare some basic signal type:
  - void
  - bool
  - std::string / std::u32string
  - int8_t / int16_t / int32_t / int64_t
  - uint8_t / uint16_t / uint32_t / uint64_t
  - float / double
  - vec2 / bvec2 / ivec2 / uivec2
  - vec3 / bvec3 / ivec3 / uivec3
  - etk::Color<unsigned char,4>
  - etk::Color<unsigned char,3>
  - etk::Color<float,4>
  - etk::Color<float,3>

To declare a signal with 'void' type:
@snippet sampleAll.cpp esignal_sample_declare_void

To declare a signal with 'int32_t' type:
@snippet sampleAll.cpp esignal_sample_declare_int

To declare a signal with 'string' type:
@snippet sampleAll.cpp esignal_sample_declare_string


Connecting on a signal                           {#esignal_connection}
======================

We have some way to connect on a signals depending on where we do the connection.

Connection and basic emition                     {#esignal_connection_base}
----------------------------

Declare the signal:
@snippet sampleAll.cpp esignal_sample_connection_simple_connection_declare

Declare a generic fuction:
@snippet sampleAll.cpp esignal_sample_connection_simple_extern_function

Connect a generic function:
@snippet sampleAll.cpp esignal_sample_connection_simple_connection_function

Or simply connect a lambda function:
@snippet sampleAll.cpp esignal_sample_connection_simple_connection_lambda

And now, we can emit a simple signal:
@snippet sampleAll.cpp esignal_sample_connection_simple_emit

You can see that all connection return a esignal::Connection value. This is an handle that can not be copiable, but only movable, While this handle is alive, the connection is allive too.
The to remove a connection, we only need to remive the handle or call the esignal::Connection::disconnect fucntion.

To disconnect a signal, it is very simple:
@snippet sampleAll.cpp esignal_sample_connection_simple_disconnect


This Will generate this simple sample code:
@snippet sampleAll.cpp esignal_sample_connection_simple


Connection on class member function                     {#esignal_connection_class}
-----------------------------------

Declare class fuction:
@snippet sampleAll.cpp esignal_sample_class_connection_callback

For direct connection, you need to have a 'const ref' on the parameter (internal helper design) to bind on the signal

Now you can nonnect the functions:
@snippet sampleAll.cpp esignal_sample_class_connection_callback_connect

This Will generate this simple sample code:
@snippet sampleAll.cpp esignal_sample_class_connection

Connection on ememory::SharedPtr<class> member function                     {#esignal_connection_shared}
----------------------------------------------------

ememory::SharedPtr have intrinsec knowledge of alive pointer, then, if you do not need to remove connection while the ememory::SharedPtr is alive, just connect it like:
@snippet sampleAll.cpp esignal_sample_shared_connection

Create new Signal                           {#esignal_create}
=================

If the signal is not in the list: @ref esignal_declare, you need to declare it yourself. This is due to optimise the compilation time,
in C++ when we inserte many template with all the implementation, the compilation time increase, the we decide to optimise the build time.

Create a new custum signal:
@snippet sampleAll.cpp esignal_sample_new_declare

Connect on the Signal:
@snippet sampleAll.cpp esignal_sample_new_lambda

Emit a Signal:
@snippet sampleAll.cpp esignal_sample_new_emit

This might work good, but at this point the compilation is OK, but not the linking ==> we need to declare the implementation of the signal:
@snippet sampleAll.cpp esignal_sample_new_register



This Will generate this simple sample code:
@snippet sampleAll.cpp esignal_sample_new