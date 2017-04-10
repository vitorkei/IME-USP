# dataClassifier.py
# -----------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).

import mostFrequent
import decisionTree
import samples
import sys
import util
import metrics

def analysis( classifier , guesses , testLabels , testData ) :
	"""
	This function is called after learning.
	Include any code that you want here to help you analyze your results.
	Input:
	- classifier is the trained classifier
	- guesses is the list of labels predicted by your classifier on the test set
	- testLabels is the list of true labels
	- testData is the list of training datapoints
   """
	correct = [ guesses[ i ] == testLabels[ i ] for i in range( len( testLabels ) ) ].count( True )
	print "%s correct out of %s (%.1f%%)" % ( correct , len( testLabels ) , 100.0 * correct / len( testLabels ) )

## =====================
## You don't have to modify any code below.
## =====================

def default(str):
	return str + ' [Default: %default]'

USAGE_STRING = """
	USAGE:      python classify.py <options>
	EXAMPLES:   (1) python classify.py
					- trains the default mostFrequent classifier on the car dataset,
					then test the classifier on test data
					(2) python classify.py -c decisionTree -d car -m gini -x 10
					- would run the decisionTree Classifier on the car training set,
					use the gini metric and set the max tree depth as 10.
					Finally, test the model learned on the car test data.
					"""

def readCommand( argv ) :
	"Processes the command used to run from the command line."
	from optparse import OptionParser
	parser = OptionParser( USAGE_STRING )

	parser.add_option('-c', '--classifier', help=default('The type of classifier'), choices=['naiveBayes', 'mostFrequent' , 'decisionTree' ] , default='mostFrequent' )
	parser.add_option('-d', '--data', help=default('Dataset to use'), choices=['car', 'ionosphere' , 'wine' ], default='car')
	parser.add_option( '-m' , '--metric' , help = default( 'Division metric' ) , choices = [ 'gini' , 'error' , 'entropy' ] , default = 'error' )
	parser.add_option( '-x' , '--maxdepth' , help = default( 'Maximum depth of the tree learned' ) , default = 200 )

	options, otherjunk = parser.parse_args( argv )
	if len( otherjunk ) != 0: raise Exception('Command line input not understood: ' + str(otherjunk))
	args = {}

	# Set up variables according to the command line input.
	print "Doing classification"
	print "--------------------"
	print "data:\t\t" + options.data
	print "classifier:\t\t" + options.classifier
	if( options.data == "car" ) : ''
	elif( options.data =="ionosphere" ) : ''
	elif( options.data =="wine" ) : ''
	else:
		print "Unknown dataset" , options.data
		print USAGE_STRING
		sys.exit( 2 )

	legalLabels = [ 0 , 1 ]

	metric = None
	if( options.classifier == "decisionTree" or options.classifier == "dt" ) :
		classifier = decisionTree.DecisionTreeClassifier( legalLabels )
		if options.metric == 'gini' :
			metric = metrics.gini
		elif options.metric == 'error' :
			metric = metrics.error
		elif options.metric == 'entropy' :
			metric = metrics.entropy
		else :
			print "Unknown metric:", options.metric
			print USAGE_STRING
			sys.exit(2)
	elif( options.classifier == "mostFrequent" or options.classifier == "mf" ) :
		classifier = mostFrequent.MostFrequentClassifier( legalLabels )
	else:
		print "Unknown classifier:", options.classifier
		print USAGE_STRING
		sys.exit(2)

	args['classifier'] = classifier
	args[ 'metric' ] = metric
	args[ 'maxdepth' ] = options.maxdepth
	return args , options

def runClassifier( args , options ) :
	classifier = args['classifier']

	# Load data
	trainingData = samples.loadDataFile( "data/%s/training_data.csv" % options.data )
	trainingLabels = samples.loadLabelsFile( "data/%s/training_labels.csv" % options.data )

	testData = samples.loadDataFile( "data/%s/test_data.csv" % options.data )
	testLabels = samples.loadLabelsFile( "data/%s/test_labels.csv" % options.data )

	# Conduct training and testing
	print "Training..."
	classifier.train( trainingData , trainingLabels , args )
	print "Testing..."
	guesses = classifier.classify( testData )
	analysis( classifier , guesses , testLabels , testData )

if __name__ == '__main__':
	# Read input
	args, options = readCommand( sys.argv[ 1: ] )
	# Run classifier
	runClassifier( args , options )
