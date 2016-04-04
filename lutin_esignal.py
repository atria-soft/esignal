#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug
import os
import lutin.multiprocess as lutinMultiprocess


def get_type():
	return "LIBRARY"

def get_desc():
	return "esignal is signal management for all class"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return ["Mr DUPIN Edouard <yui.heero@gmail.com>"]

def get_version():
	return [0,1,"dev"]

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_extra_compile_flags()
	my_module.add_src_file([
		'esignal/debug.cpp',
		'esignal/Connection.cpp',
		'esignal/Interface.cpp',
		'esignal/Base.cpp',
		'esignal/details/LockSharedPtrRef.cpp',
		'esignal/details/RefCount.cpp',
		'esignal/details/Signal.cpp',
		'esignal/details/ISignal.cpp',
		])
	my_module.add_header_file([
		'esignal/debug.h',
		'esignal/Interface.h',
		'esignal/Base.h',
		'esignal/Signal.h',
		'esignal/ISignal.h',
		'esignal/LockSharedPtrRef.h',
		'esignal/RefCount.h',
		'esignal/Connection.h',
		'esignal/details/Signal.hxx',
		'esignal/details/ISignal.hxx',
		'esignal/details/LockSharedPtrRef.hxx',
		'esignal/details/RefCount.hxx',
		])
	my_module.compile_version("c++", 2011)
	my_module.add_module_depend([
	    'etk',
	    'ememory'
	    ])
	my_module.add_path(tools.get_current_path(__file__))
	my_module.compile_flags('c++', [
		"-DESIGNAL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	my_module.add_tools(['esignal-test'])
	return my_module

