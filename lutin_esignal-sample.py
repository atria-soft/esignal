#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import datetime

def get_type():
	return "BINARY"

def get_sub_type():
	return "SAMPLE"

def get_desc():
	return "e-signal sample 1"

def get_licence():
	return "APACHE-2"

def get_compagny_type():
	return "com"

def get_compagny_name():
	return "atria-soft"

def get_maintainer():
	return "authors.txt"

def create(target, module_name):
	my_module = module.Module(__file__, module_name, get_type())
	my_module.add_src_file([
		'sample/sampleAll.cpp'
		])
	my_module.add_depend(['esignal', 'test-debug'])
	return my_module

