###########################################################
# Program:		test_kMatMult.py
# Purpose:		EE392Q assignment07 testing script
# Author:		Nora Tarano (ntarano@stanford.edu)
# Execution:	python test_kMatMult.py
###########################################################

import os, sys

# CHECK: Make sure Kestrel is in your $PATH

# TODO: Set the path to your assignment here
assignment_path = ''

if assignment_path == '':
	print "Set the assignment_path in the script"
	sys.exit(1)

#########################################################################################
# Simple tests
# * simple0 - N=2, H=2, M=2, random
# * simple1 - N=4, H=4, M=1, random
# * simple2 - N=8, H=2, M=4, random
# * simple3 - N=4, H=8, M=4, random
# * simple4 - N=1, H=3, M=1, random
#########################################################################################
num_simple_tc = 5
NPEs = [2, 1, 4, 4, 1]
for i in range(0, num_simple_tc):
	print 'Running simple' + str(i)
	os.system('kestrel ' + assignment_path + 'kMatMult.ko simple' + str(i) + '_in kout ' + str(NPEs[i]) + ' &>/dev/null')
	out = os.system('diff kout simple' + str(i) + '_out &> simple' + str(i) + '_diff')
	if out == 0:
		os.system('rm -rf simple' + str(i) + '_diff')
		print "PASS"
	else:
		print "FAIL : diff stored in simple" + str(i) + "_diff"

################################################################################################
# Corner tests
# * corner0 - N=0, H=0, M=0
# * corner1 - N=1, H=256, M=257, random
# * corner2 - N=257, H=1, M=257, random
# * corner3 - N=257, H=256, M=257, random
# * corner4 - N=1, H=1, M=257, random
# * corner5 - N=1, H=256, M=1, random
# * corner6 - N=1, H=1, M=1, random
################################################################################################
num_corner_tc = 7
NPEs = [6,257,257,257,257,1,1]
for i in range(0, num_corner_tc):
	print 'Running corner' + str(i)
	os.system('kestrel ' + assignment_path + 'kMatMult.ko corner' + str(i) + '_in kout ' + str(NPEs[i]) + ' &>/dev/null')
	out = os.system('diff kout corner' + str(i) + '_out &> corner' + str(i) + '_diff')
	if out == 0:
		os.system('rm -rf corner' + str(i) + '_diff')
		print "PASS"
	else:
		print "FAIL : diff stored in corner" + str(i) + "_diff"

os.system('rm -rf kout')
