import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier

def has_one_level( tree_node , depth = 0 ) :
	if depth > 1 : return False
	if tree_node.leftchild :
		return has_one_level( tree_node.leftchild , depth + 1 )
	if tree_node.rightchild :
		return has_one_level( tree_node.rightchild , depth + 1 )
	return True

def run( config , codeRoot = '' ) :
	classifier = DecisionTreeClassifier( [ 0 , 1 ] )
	print " ========= %s ========= " % config[ 'header' ]
	pointsPerCase = int( config[ 'pointsPerCase' ] )

	args = {}
	args[ 'metric' ] = metrics.error
	args[ 'maxdepth' ] = 1

	size = len( config[ 'dataset' ] )
	points = 0
	first_error = ''
	for i in xrange( size ) :
		data = config[ 'dataset' ][ i ]
		print "\tDataset: %s" % data
		# Load data
		trainingData = samples.loadDataFile( os.path.join( codeRoot , "data/%s/training_data.csv" % data ) )
		trainingLabels = samples.loadLabelsFile( os.path.join( codeRoot , "data/%s/training_labels.csv" % data ) )
	
		# Conduct training and testing
		classifier.train( trainingData , trainingLabels , args )
		tree = classifier.tree
		best_split = int( config[ 'best_split' ][ i ] )
		test_passed = True
		if tree.column != best_split :
			error = 'WRONG SPLIT: Correct: %s , Returned: %s' % ( best_split , tree.column )
			if not first_error : first_error = error
			print '\t\t%s' % error
			test_passed = False

		if not has_one_level( tree ) :
			error = 'The tree does not have one level'
			if not first_error : first_error = error
			print '\t\t%s' % error
			test_passed = False

		if test_passed :
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
