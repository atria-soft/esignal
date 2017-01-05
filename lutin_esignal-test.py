#!/usr/bin/python
import lutin.debug as debug
import lutin.tools as tools


def get_type():
	return "BINARY"

def get_sub_type():
	return "TEST"

def get_desc():
	return "e-signal test software"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def configure(target, my_module):
	my_module.add_src_file([
	    'test/main.cpp',
	    'test/declareSignals.cpp',
	    'test/test_signal_counter.cpp',
	    'test/test_signal_class_func.cpp',
	    'test/test_signal_recursive.cpp',
	    'test/test_signal_shared_ptr_func.cpp',
	    'test/test_signal_static_func.cpp',
	    'test/test_isignal.cpp'
	    ])
	my_module.add_depend([
	    'esignal',
	    'gtest',
	    'test-debug'
	    ])
	return True

