#! /usr/bin/env python
# encoding: utf-8

#  This is the makefile for libif97.

# some useful tips https://ftp.samba.org/pub/unpacked/waf/docs/book/cprog.txt


# in order to run waf, you will need a working copy of Python2 or Python3

# Assuming you are using either windows Powershell or a bash terminal
#  to build libif97, from the directory containing this file simply:
#  $ ./waf configure build



#from waftools import ccenv  #needed later for cross compiling.  just a reminder
# see https://pythonhosted.org/waftools/waftools.html

APPNAME = 'libif97'
VERSION = '0.1pre'

top = '.'   # root of project is the directory this script sits in
out = 'build'   # where the built programs will be put.  it must not be . or..


# add command line options
def options(opt):
	opt.load('compiler_c')
	# opt.load('compiler_cxx') #uncomment if there is c++ code
	opt.load('doxygen')
	opt.load('swig')
	#opt.load('python')

	opt.add_option('--nothread', action='store_false',  dest='thread', default=True,  help='switch multithreadding support off')
	opt.add_option('--nodoc', action='store_false',  dest='doxygen', default=True,  help='switch documentation generation off')
	opt.add_option('--nopybindings', action='store_false',  dest='swig_pyton', default=True,  help='switch python bindings generation off')	

def configure(cnf):
	cnf.load('compiler_c')
	# cnf.load('compiler_cxx') #uncomment if there is c++ code
	cnf.load('doxygen')
	cnf.load('swig')
	#cnf.load('python')

	
	print ('Compile with multithreadding support	: ' , cnf.options.thread)
	
	cnf.env.THREAD = cnf.options.thread
	
	
#	cnf.check(features='c cprogram', lib=['m'], cflags=['-Wall'],  uselib_store='M')
#	cnf.check(features='c cprogram', lib=['gomp'], cflags=['-Wall', '-fopenmp'],  uselib_store='GOMP')

	print ("c compiler is " + cnf.env.CC_NAME,cnf.env.CC)
	if cnf.env.CC_NAME == "gcc":
		cnf.env.CFLAGS = ['-g', '-Wall']
	
		#link time optimisation
		#cnf.env.append_unique('CFLAGS', ['-flto'])
	
		#normal release level compiler optimisation
		#cnf.env.append_unique('CFLAGS', ['-O2'])
	
		#compiler optimisation for executable size
		#cnf.env.append_unique('CFLAGS', ['-Os'])
	
	if  cnf.env.THREAD == True: 
		if cnf.env.CC_NAME == "gcc":
			cnf.env.append_unique('CFLAGS', ['-fopenmp'])
		if cnf.env.CC_NAME == "msvc":
			cnf.env.append_unique('CFLAGS', ['/openmp'])
		else:
			pass


	#cnf.env.CXXFLAGS = cnf.env.CFLAGS 

	#ctx.env.append_value('CXXFLAGS', ['-O2', '-g']) 
	#ctx.env.append_unique('CFLAGS', ['-g', '-O2'])
	#ctx.env.prepend_value('CFLAGS', ['-O3']) 

	print ('CFLAGS					:',  cnf.env.CFLAGS)



def build(bld):
	# so that bld.program etc. know what to do for c source code without bespoke rules
	bld.load('compiler_c')
	bld.load('doxygen')
	bld.load('swig')
	#bld.load('python')
	# bld.load('compiler_cxx') #uncomment if there is c++ code
	
	
	# bld.program 	for an application
	# bld.stlib	 	for a static library
	# bld.shlib	 	for a shared library
	# bld.objects	to just compile to object code
	
	# To make a bespoke build rule, it takes the following form
	# bld(rule='cp ${SRC} ${TGT}', source='region2test_lnx', target='region2test.exe')
	
	#bld(rule='doxygen ${SRC} ${TGT}', source='if97-doxy-config', target='doc')

	#gcc needs to be told to link to libm but msvc fails if told to do so
	if97libs =[]
#	if bld.env.CC_NAME != "msvc":
#		if97libs.append_unique('m')
#	if bld.env.THREAD== "true":
#		if97libs.append_unique('gomp')
	
	
	wsCompatLibs = if97libs
	wsCompatLibs.append('units')
	
	r4TestLibs=if97libs
	try:
		r4TestLibs.remove('gomp')
	except:
		pass
	
	
	bld.stlib(source = 'solve.c', target='solve')
	bld.stlib(source='units.c', target='units')
	
	bld.stlib(source='IF97_common.c IF97_Region1.c  IF97_Region1bw.c \
	IF97_Region2.c IF97_Region2bw.c IF97_Region2_met.c	\
	IF97_Region3.c IF97_Region3bw.c IF97_Region4.c 	IF97_Region5.c IF97_B23.c \
	iapws_surftens.c if97_lib.c', target='if97', lib=['solve']) 

	
	bld.stlib(source='winsteam_compatibility.c', target='winsteam_compatibility', lib = list(wsCompatLibs))
	
	bld.stlib(source='IF97_B23_test.c',     target='b23test', use=['if97', 'M', 'GOMP'])
	bld.stlib(source='IF97_Region1_test.c', target='region1_test',  use=['if97', 'M', 'GOMP'])
	bld.stlib(source='IF97_Region2_test.c', target='region2_test',  use=['if97', 'M', 'GOMP'])
	bld.stlib(source='IF97_Region3_test.c', target='region3_test',  use=['if97', 'M', 'GOMP'])
	bld.stlib(source='IF97_Region4_test.c', target='region4_test', use=['if97', 'M', 'GOMP'])
	bld.stlib(source='IF97_Region5_test.c', target='region5_test', use=['if97', 'M'])
	bld.stlib(source='solve_test.c', target='solve_test', use=['if97', 'M', 'GOMP', 'solve'])
	
	
	
	bld.program(source='if97_lib_test.c', target='if97_lib_test', use=['if97', 'b23test', 'region1_test', \
	'region2_test', 'region3_test', 'region4_test', 'region5_test', 'solve_test', 'winsteam_compatibility', 'M'] , lib = ['units', 'solve'])



	
	#generate the osteam_wrap c source using swig
	# some inspiration here https://github.com/waf-project/waf/blob/master/playground/swig/wscript
	#NOT WORKING
	#bld(
	#	features = 'cshlib pyext',
	#	source = 'osteam.i',
	#	target = 'osteam_pylib',
	#	swig_flags = '-python',
	#	use = 'winsteam_compatibility',
	#	)
		



		

	
	# compile files under development without linking
	#bld.objects(source='IF97_Region4.c', target='myobjects')
	
	# build documentation
	if bld.env.DOXYGEN:
		bld(features="doxygen", doxyfile='Doxyfile')


def dist(dst):
	
	# unhash to change the compression to zip
	# ctx.algo = 'zip'  # unhash to change the compression to zip

	# ctx.excl:  what to exclude from the distribution bundle
	dst.excl = ' **/.waf-1* **/*~ **/*.pyc **/*.swp **/.lock-w*'
