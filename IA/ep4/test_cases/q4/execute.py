import sys
import samples
import metrics
import os.path
from decisionTree import DecisionTreeClassifier , DecisionNode

def get_tree( dataset , directory ) :
	tree = DecisionNode()
	stack = [ tree ]
	current = stack.pop()
	with open( os.path.join( directory , 'test_cases' , 'q4' , '%s.txt' % dataset ) , 'r' ) as f :
		for line in f :
			pos = line.find( 'Split' )
			if pos >= 0 :
				current.column = int( line.strip().split( ':' )[ 1 ] )
				current.leftchild = DecisionNode()# depth = current.depth + 1 )
				current.rightchild = DecisionNode()# depth = current.depth + 1 )
				stack.append( current.rightchild )
				stack.append( current.leftchild )
			pos = line.find( 'LEFT' )
			if pos >= 0 :
				current = stack.pop()
			pos = line.find( 'RIGHT' )
			if pos >= 0 :
				current = stack.pop()
			pos = line.find( 'Label' )
			if pos >= 0 :
				current.label = int( line.strip().split( ':' )[ 1 ] )
	return tree

def compare( tree_a , tree_b ) :
	''' Check if nodes have same label '''
	if tree_a.label is not None :
		if tree_b.label is None :
			return 3 , 'The correct tree splits more nodes'
		elif tree_a.label != tree_b.label :
			return 4 , 'Some nodes does not have same label'
		else :
			return 0 , 'Correct'
	''' Check if they have same levels '''
	if tree_a.leftchild :
		if not tree_b.leftchild :
			return 1 , 'The correct tree has less levels'
		else :
			status , error = compare( tree_a.leftchild , tree_b.leftchild )
			if status != 0 : return status , error
			status , error = compare( tree_a.rightchild , tree_b.rightchild )
			if status != 0 : return status , error
			return 0 , 'Correct'
	if tree_b.leftchild :
		if not tree_a.leftchild :
			return 2 , 'The correct tree has more levels'
		else :
			status , error = compare( tree_a.leftchild , tree_b.leftchild )
			if status != 0 : return status , error
			status , error = compare( tree_a.rightchild , tree_b.rightchild )
			if status != 0 : return status , error
			return 0 , 'Correct'

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
	args[ 'maxdepth' ] = int( config[ 'maxdepth' ] )

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
		status , error = compare( tree , get_tree( data , codeRoot ) )
		if status != 0 :
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
