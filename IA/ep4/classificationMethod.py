# classificationMethod.py
# -----------------------
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


# This file contains the abstract class ClassificationMethod

class ClassificationMethod:
    """
    ClassificationMethod is the abstract superclass of
     - MostFrequentClassifier
     - DecisionTreeClassifier

    As such, you need not add any code to this file.  You can write
    all of your implementation code in the files for the individual
    classification methods listed above.
    """
    def __init__( self , legalLabels ) :
        """
        The set of legal labels in all datasets will be 0,1
        """
        self.legalLabels = legalLabels


    def train( self , trainingData , trainingLabels , args ) :
        """
        This is the supervised training function for the classifier. A set of
        labeled data is passed in and a dictionary with specific classifier
		  parameters.

        Many types of classifiers have a common training structure in practice: using
        training data for the main supervised training loop.
        """
        abstract

    def classify(self, data):
        """
        This function returns a list of labels, each drawn from the set of legal labels
        provided to the classifier upon construction.

        To make the classifier generic to multiple problems, the data should be represented
        as lists of Counters containing feature descriptions and their counts.
        """
        abstract
