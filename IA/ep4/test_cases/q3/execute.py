import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier

def run( config , codeRoot = '' ) :
	classifier = DecisionTreeClassifier( [ 0 , 1 ] )
	print " ========= %s ========= " % config[ 'header' ]
	pointsPerCase = int( config[ 'pointsPerCase' ] )

	size = len( config[ 'data' ] )
	points = 0
	first_error = ''

	''' Check if it is empty '''
	data = eval( config[ 'data' ][ 0 ] )
	labels = eval( config[ 'label' ][ 0 ] )
	maxdepth = int( config[ 'maxdepth' ][ 0 ] )
	classifier.maxdepth = maxdepth
	print '\tCase #1: DATA(%s), LABELS(%s)' % ( data , labels )
	if not classifier.isLeaf( data , labels , 2 ) :
		error = 'WRONG: Check if dataset is empty'
		if not first_error : first_error = error
		print '\t\t%s' % error
	else :
		points += pointsPerCase
		print '\t\tOK'

	''' Check if all instances have same label '''
	data = eval( config[ 'data' ][ 1 ] )
	labels = eval( config[ 'label' ][ 1 ] )
	maxdepth = int( config[ 'maxdepth' ][ 1 ] )
	classifier.maxdepth = maxdepth
	print '\tCase #2: DATA(%s), LABELS(%s)' % ( data , labels )
	if not classifier.isLeaf( data , labels , 2 ) :
		error = 'WRONG: Check if all instances have same label'
		if not first_error : first_error = error
		print '\t\t%s' % error
	else :
		points += pointsPerCase
		print '\t\tOK'

	''' Check if all instances are the same '''
	data = eval( config[ 'data' ][ 2 ] )
	labels = eval( config[ 'label' ][ 2 ] )
	maxdepth = int( config[ 'maxdepth' ][ 2 ] )
	classifier.maxdepth = maxdepth
	print '\tCase #3: DATA(%s), LABELS(%s)' % ( data , labels )
	if not classifier.isLeaf( data , labels , 2 ) :
		error = 'WRONG: Check if all instances are the same'
		if not first_error : first_error = error
		print '\t\t%s' % error
	else :
		points += pointsPerCase
		print '\t\tOK'

	''' Check maximum depth '''	
	data = eval( config[ 'data' ][ 3 ] )
	labels = eval( config[ 'label' ][ 3 ] )
	maxdepth = int( config[ 'maxdepth' ][ 3 ] )
	classifier.maxdepth = maxdepth
	print '\tCase #4: DATA(%s), LABELS(%s)' % ( data , labels )
	if not classifier.isLeaf( data , labels , 3 ) :
		error = 'WRONG: Check maximum depth parameter. Maximum: %s, Current: %s' % ( maxdepth , 3 )
		if not first_error : first_error = error
		print '\t\t%s' % error
	else :
		points += pointsPerCase
		print '\t\tOK'

	needed_all_tests_passed = int( config[ 'totalQuestion' ] ) > 0
	if needed_all_tests_passed and first_error :
		points = 0
		test_error = 'Your code does not pass all tests'
	else :
		test_error = ''

	return ( points , test_error )

if __name__ == '__main__' :
	run()
