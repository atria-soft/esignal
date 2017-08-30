#!/usr/bin/python
import lutin.tools as tools
import lutin.debug as debug


def get_type():
	return "LIBRARY"

def get_desc():
	return "esignal is signal management for all class"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def get_version():
	return "version.txt"

def configure(target, my_module):
	my_module.add_extra_flags()
	my_module.add_src_file([
	    'esignal/debug.cpp',
	    'esignal/Connection.cpp',
	    'esignal/InterfaceData.cpp',
	    'esignal/Base.cpp',
	    'esignal/details/Signal.cpp',
	    ])
	my_module.add_header_file([
	    'esignal/debug.hpp',
	    'esignal/Interface.hpp',
	    'esignal/InterfaceData.hpp',
	    'esignal/Base.hpp',
	    'esignal/Signal.hpp',
	    'esignal/Connection.hpp',
	    'esignal/details/Signal.hxx',
	    ])
	my_module.compile_version("c++", 2011)
	my_module.add_depend([
	    'etk',
	    'ememory'
	    ])
	my_module.add_path(".")
	my_module.add_flag('c++', [
		"-DESIGNAL_VERSION=\"\\\"" + tools.version_to_string(get_version()) + "\\\"\""
		])
	my_module.add_optionnal_depend('echrono', compilation_flags=["c++", "-DESIGNAL_BUILD_WITH_ECHRONO"])
	my_module.add_tools(['esignal-test'])
	return True

