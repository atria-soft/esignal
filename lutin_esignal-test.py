#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime


def get_type():
	return "BINARY"

def get_sub_type():
	return "TEST"

def get_desc():
	return "e-signal test software"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
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
	my_module.add_module_depend(['esignal', 'gtest', 'test-debug'])
	return my_module

