#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'if97lib'
VERSION = '0.1pre'

top = '.'   # root of project is the directory this script sits in
out = 'build'   # where the built programs will be put.  it must not be . or..


# add command line options
def options(opt):
	opt.load('compiler_c')
	opt.add_option('--nothread', action='store_false',  dest='thread', default=True,  help='switch multithreadding support off')
        

def configure(cnf):
	cnf.load('compiler_c')
	
	print 'Compile with multithreadding support	: ' , cnf.options.thread
	
	cnf.env.THREAD = cnf.options.thread
        
	cnf.env.CFLAGS = ['-g', '-Wall']
	
	if  cnf.env.THREAD == True: 
		cnf.env.append_unique('CFLAGS', ['-fopenmp'])
	#cnf.env.CXXFLAGS = cnf.env.CFLAGS 


        #ctx.env.append_value('CXXFLAGS', ['-O2', '-g']) 
        #ctx.env.append_unique('CFLAGS', ['-g', '-O2'])
        #ctx.env.prepend_value('CFLAGS', ['-O3']) 

	print 'CFLAGS					:',  cnf.env.CFLAGS
	# print(type(cnf.env))
	#print(cnf.env)
        
	pass


def build(bld):
	bld.load('compiler_c')
	
	# bld.program 	for an application
	# bld.stlib	 	for a static library
	# bld.shlib	 	for a shared library
	
	
	#bld(rule='cp ${SRC} ${TGT}', source='region2test_lnx', target='region2test.exe')
	
	bld.program(source='region2_test.c', target='region2_test', use='if97lib', lib = ['m', 'gomp']) 
	bld.stlib(source='IF97_common.c IF97_Region2.c IF97_Region2_met.c', target='if97lib') 
	#bld.shlib(source='b.c', target='mystlib', use='myobjects') 
	#bld.objects(source='c.c', target='myobjects')


def dist(dst):
	
	# unhash to change the compression to zip
	# ctx.algo = 'zip'  # unhash to change the compression to zip

	# ctx.excl:  what to exclude from the distribution bundle
	dst.excl = ' **/.waf-1* **/*~ **/*.pyc **/*.swp **/.lock-w*'
