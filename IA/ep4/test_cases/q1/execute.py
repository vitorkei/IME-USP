import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier

def compare( val_a , val_b ) :
	EPS = 1e-5
	if val_a + EPS < val_b : return -1
	if val_a - EPS > val_b : return 1
	return 0

def get_metric( metric ) :
	if metric == 'gini' :
		return metrics.gini
	elif metric == 'error' :
		return metrics.error
	elif metric == 'entropy' :
		return metrics.entropy
	raise Exception( 'There is no metric called: %s' % metric )

def run( config , codeRoot = '' ) :
	classifier = DecisionTreeClassifier( [ 0 , 1 ] )
	print " ========= %s ========= " % config[ 'header' ]
	pointsPerCase = int( config[ 'pointsPerCase' ] )

	num_cases = len( config[ 'case' ] )
	points = 0
	first_error = ''
	for metric in config[ 'metric' ] :
		mymet = get_metric( metric )
		print "\tMetric: %s" % metric
		results = [ float( val ) for val in config[ 'results_%s' % metric ] ]
		for i in xrange( num_cases ) :
			data = eval( config[ 'case' ][ i ] )
			print "\t\tCase: %s" % data
			resp = mymet( data )

			if compare( resp , results[ i ] ) == 0 :
				points += pointsPerCase
				print '\t\t\tOK'
			else :
				error = 'ERROR: Correct: %s, Returned: %s' % ( results[ i ] , resp )
				print '\t\t\t%s' % error
				if not first_error : first_error = error
	# Check if need to pass all tests
	needed_all_tests_passed = int( config[ 'totalQuestion' ] ) > 0
	if needed_all_tests_passed and first_error :
		points = 0
		test_error = 'Your code does not pass all tests'
	else :
		test_error = ''
	return ( points , test_error )

if __name__ == '__main__' :
	run()
