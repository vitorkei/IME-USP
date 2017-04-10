# classificationTestClasses.py
# ----------------------------
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


from hashlib import sha1
import testClasses
# import json

from collections import defaultdict
from pprint import PrettyPrinter
pp = PrettyPrinter()

import random, math, traceback, sys, os
# import autograder
# import grading

import classify, samples
import metrics

VERBOSE = False

# Test classes
# ------------

DATASETS = {
    'car' : lambda : samples.readData( 'car' )
}

DATASETS_LEGAL_LABELS = {
	'car' : [ 0 , 1 ]
}

def getAccuracy(data, classifier,args):
    trainingData, trainingLabels, testData, testLabels, = data
    classifier.train(trainingData, trainingLabels,args)
    guesses = classifier.classify(testData)
    correct = [guesses[i] == testLabels[i] for i in range(len(testLabels))].count(True)
    acc = 100.0 * correct / len(testLabels)
    serialized_guesses = ", ".join([str(guesses[i]) for i in range(len(testLabels))])
    print str(correct), ("correct out of " + str(len(testLabels)) + " (%.1f%%).") % (acc)
    return acc, serialized_guesses

class GradeClassifierTest(testClasses.TestCase):

    def __init__(self, question, testDict):
        super(GradeClassifierTest, self).__init__(question, testDict)

        self.classifierModule = testDict['classifierModule']
        self.classifierClass = testDict['classifierClass']
        self.datasetName = testDict['datasetName']

        self.accuracyScale = int(testDict['accuracyScale'])
        self.accuracyThresholds = [int(s) for s in testDict.get('accuracyThresholds','').split()]
        self.exactOutput = testDict['exactOutput'].lower() == "true"

        self.max_depth = int(testDict['max_depth']) if 'max_depth' in testDict else 10000
        self.metricFunction = testDict['metric'] if 'metric' in testDict else 'error'

        self.maxPoints = len(self.accuracyThresholds) * self.accuracyScale


    def grade_classifier(self, moduleDict):
        metricFunction = getattr(metrics, self.metricFunction)
        data = DATASETS[ self.datasetName ]()
        legalLabels = DATASETS_LEGAL_LABELS[ self.datasetName ]

        classifierClass = getattr(moduleDict[self.classifierModule], self.classifierClass)

        classifier = classifierClass(legalLabels)

        args = { 'metric' : metricFunction , 'maxdepth' : self.max_depth }
        return getAccuracy(data, classifier, args)


    def execute(self, grades, moduleDict, solutionDict):
        accuracy, guesses = self.grade_classifier(moduleDict)

        # Either grade them on the accuracy of their classifer,
        # or their exact
        if self.exactOutput:
            gold_guesses = solutionDict['guesses']
            if guesses == gold_guesses:
                totalPoints = self.maxPoints
            else:
                self.addMessage("Incorrect classification after training:")
                self.addMessage("  student classifications: " + guesses)
                self.addMessage("  correct classifications: " + gold_guesses)
                totalPoints = 0
        else:
            # Grade accuracy
            totalPoints = 0
            for threshold in self.accuracyThresholds:
                if accuracy >= threshold:
                    totalPoints += self.accuracyScale

            # Print grading schedule
            self.addMessage("%s correct (%s of %s points)" % (accuracy, totalPoints, self.maxPoints))
            self.addMessage("    Grading scheme:")
            self.addMessage("     < %s:  0 points" % (self.accuracyThresholds[0],))
            for idx, threshold in enumerate(self.accuracyThresholds):
                self.addMessage("    >= %s:  %s points" % (threshold, (idx+1)*self.accuracyScale))

        return self.testPartial(grades, totalPoints, self.maxPoints)

    def writeSolution(self, moduleDict, filePath):
        handle = open(filePath, 'w')
        handle.write('# This is the solution file for %s.\n' % self.path)

        if self.exactOutput:
            _, guesses = self.grade_classifier(moduleDict)
            handle.write('guesses: "%s"' % (guesses,))

        handle.close()
        return True
