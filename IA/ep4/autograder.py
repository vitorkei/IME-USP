# imports from python standard library
import grading
import imp
import optparse
import os
import re
import sys
import projectParams
import random
random.seed( 0 )

# register arguments and set default values
def readCommand( argv ) :
	parser = optparse.OptionParser(description = 'Run public tests on student code')
	parser.add_option('--test-directory',
							dest = 'testRoot',
							default = 'test_cases',
							help = 'Root test directory which contains subdirectories corresponding to each question')
	parser.add_option('--code-directory',
							dest = 'codeRoot',
							default = os.path.dirname( os.path.abspath( __file__ ) ) ,
							help = 'Root code directory which contains student implementations')
	parser.add_option('--question', '-q',
							dest = 'gradeQuestion',
							default = None,
							help = 'Grade one particular question.')
	(options, args) = parser.parse_args(argv)
	return options

def readFile(path, root=""):
	"Read file from disk at specified path and return as string"
	with open(os.path.join(root, path), 'r') as handle:
		return handle.read()

def evaluate( question , codeRoot = '' ) :
	import test_runner
	response , error = test_runner.execute( question , codeRoot = codeRoot )
	if not response :
		print 'NOT PASS: %s' % error
	else :
		print 'PASS: %s points' % response

def get_all_questions( testRoot ) :
	resp = readFile( 'CONFIG' , root = testRoot )
	return resp.split()

if __name__ == '__main__' :
	options = readCommand( sys.argv )
	sys.path.append( options.testRoot )
	sys.path.append( options.codeRoot )
	print options

	if not options.gradeQuestion :
		questions = get_all_questions( options.testRoot )
	else :
		questions = [ 'q%s' % options.gradeQuestion ]
	
	for q in questions :
		evaluate( q , codeRoot = options.codeRoot )
		print
