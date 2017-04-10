import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier , DecisionNode

def compare( val_a , val_b ) :
	EPS = 1e-5
	if val_a + EPS < val_b : return -1
	if val_a - EPS > val_b : return 1
	return 0

def is_good_classifier( guesses , testLabels , min_accuracy_required ) :
	correct = [ guesses[ i ] == testLabels[ i ] for i in range( len( testLabels ) ) ].count( True )
	accuracy = 100.0 * correct / len( testLabels )
	if compare( accuracy , min_accuracy_required ) >= 0 :
		return True
	return False

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

	args = {}
	args[ 'metric' ] = get_metric( config[ 'metric' ] )

	size = len( config[ 'dataset' ] )
	points = 0
	first_error = ''
	for i in xrange( size ) :
		data = config[ 'dataset' ][ i ]
		print "\tDataset: %s" % data
		# Load data
		trainingData = samples.loadDataFile( os.path.join( codeRoot , "data/%s/training_data.csv" % data ) )
		trainingLabels = samples.loadLabelsFile( os.path.join( codeRoot , "data/%s/training_labels.csv" % data ) )
	
		# Load test
		testData = samples.loadDataFile( os.path.join( codeRoot , "data/%s/test_data.csv" % data ) )
		testLabels = samples.loadLabelsFile( os.path.join( codeRoot , "data/%s/test_labels.csv" % data ) )

		# Conduct training and testing
		args[ 'maxdepth' ] = int( config[ 'maxdepth' ][ i ] )
		classifier.train( trainingData , trainingLabels , args )
		guesses = classifier.classify( testData )
		min_accuracy_required = float( config[ 'accuracy' ][ i ] )
		if is_good_classifier( guesses , testLabels , min_accuracy_required ) :
			print '\t\tOK'
			points += pointsPerCase
		else :
			points += ( pointsPerCase / 2. )
			print '\t\tMost frequent classifier is better'
			
	needed_all_tests_passed = int( config[ 'totalQuestion' ] ) > 0
	if needed_all_tests_passed and first_error :
		points = 0
		test_error = 'Your code does not pass all tests'
	else :
		test_error = ''

	return ( points , test_error )

if __name__ == '__main__' :
	run()
