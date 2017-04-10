# samples.py
# ----------
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

import util

# Data processing, cleanup and display functions

def loadDataFile(filename):
	"""
	Reads domain's instances from a file and returns a list of objects.
	"""
	fin = readlines(filename)
	items = convertToInteger( [ instance for instance in fin ] )
	return items

import zipfile
import os
def readlines(filename):
	"Opens a file or reads it from the zip archive data.zip"
	if( os.path.exists( filename ) ) :
		return [ l[ :-1 ].split( ',' ) for l in open( filename ).readlines() ]
	else :
		z = zipfile.ZipFile( 'data.zip' )
	return z.read( filename ).split( '\n' )

def loadLabelsFile(filename):
	"""
	Reads labels from a file and returns a list of integers.
	"""
	fin = readlines(filename)
	labels = convertToInteger( [ lbl[ 0 ] for lbl in fin ] )
	return labels

def readData( datasetname ) :
   training_data = loadDataFile( "data/%s/training_data.csv" % datasetname )
   training_label = loadLabelsFile( "data/%s/training_labels.csv" % datasetname )
   test_data = loadDataFile( "data/%s/test_data.csv" % datasetname )
   test_label = loadLabelsFile( "data/%s/test_labels.csv" % datasetname )
   return ( training_data , training_label , test_data , test_label )

import cPickle
import textDisplay
def convertToInteger(data):
	"""
	Helper function for file reading.
	"""
	if type( data ) != type( [] ) :
		return int( data )
	return map( convertToInteger , data )

# Testing
def _test() :
	import doctest
	doctest.testmod() # Test the interactive sessions in function comments
	n = 10
	items = loadDataFile( "data/car/training_data.csv" )
	labels = loadLabelsFile( "data/car/training_labels.csv" )
	for i in range( n ) :
		print items[ i ] , labels[ i ]

if __name__ == "__main__" :
	_test()
