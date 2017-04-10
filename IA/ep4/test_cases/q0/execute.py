import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier

def run( config , codeRoot = '' ) :
	classifier = DecisionTreeClassifier( [ 0 , 1 ] )
	print " ========= %s ========= " % config[ 'header' ]

	args = {}
	args[ 'metric' ] = metrics.error
	args[ 'maxdepth' ] = 0

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
		if not tree.leftchild and not tree.rightchild :
			label = int( config[ 'label' ][ i ] )
			if tree.label != label :
				error = 'The tree is not classifying by the most frequent label: %s' % label
				if not first_error : first_error = error
				print '\t\t%s' % error
			else :
				points += int( config[ 'pointsPerCase' ] )
				print '\t\tOK'
		else :
			error = 'The tree has one or more levels'
			if not first_error : first_error = error
			print '\t\t%s' % error
	needed_all_tests_passed = int( config[ 'totalQuestion' ] ) > 0
	if needed_all_tests_passed and first_error :
		points = 0
		test_error = 'Your code does not pass all tests'
	else :
		test_error = ''

	return ( points , test_error )

if __name__ == '__main__' :
	run()
