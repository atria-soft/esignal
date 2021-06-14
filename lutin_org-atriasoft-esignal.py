#!/usr/bin/python
import realog.debug as debug
import lutin.tools as tools
import realog.debug as debug
import lutin.image as image
import os
import lutin.multiprocess as lutinMultiprocess


def get_type():
	return "LIBRARY_DYNAMIC"

def get_desc():
	return "Ewol Tool Kit"

def get_licence():
	return "MPL-2"

def get_compagny_type():
	return "org"

def get_compagny_name():
	return "atria-soft"

#def get_maintainer():
#	return "authors.txt"

#def get_version():
#	return "version.txt"

def configure(target, my_module):

	my_module.add_src_file([
	    'src/module-info.java',
	    'src/org/atriasoft/esignal/ConnectionRemoveInterface.java',
	    'src/org/atriasoft/esignal/GenericSignalInstrumented.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementTRefConsumerWeakObject.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementTRefConsumerWeakConnection.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementTRefConsumer.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementURefConsumerWeakObject.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementURefConsumerWeakObjectWeakConnection.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementInterface.java',
	    'src/org/atriasoft/esignal/internal/ConnectedElementTWeakConsumer.java',
	    'src/org/atriasoft/esignal/SignalEmpty.java',
	    'src/org/atriasoft/esignal/ISignalEmpty.java',
	    'src/org/atriasoft/esignal/Signal.java',
	    'src/org/atriasoft/esignal/ISignal.java',
	    'src/org/atriasoft/esignal/Connection.java',
	    'src/org/atriasoft/esignal/GenericSignal.java',
	    ])
	my_module.add_path('src/', type='java')
	
	my_module.add_depend([
	    'org-atriasoft-etk'
	    ])
	
	#my_module.add_path([
	#    'lib/spotbugs-annotations-4.2.2.jar'
	#    ],
	#    type='java',
	#    export=True
	#);
	my_module.add_flag('java', "RELEASE_15_PREVIEW");
	
	return True

