# -*- coding: utf-8 -*-

# Nome: Vítor Kei Taira Tamada
# NUSP: 8516250

from copy import deepcopy as copy
import util
import classificationMethod

class DecisionNode :
	def __init__( self , column = -1 , value = None , label = None , leftchild = None , rightchild = None ) :
		self.column = column
		self.value = value
		self.label = label
		self.leftchild = leftchild
		self.rightchild = rightchild

	def __str__( self ) :
		"""
		Return a string representing the structure of DecisionNode
		This function will be implicitly called when printed a DecisionNode object using print function
		"""
		"*** YOUR CODE HERE (Optional) ***"
		util.raiseNotDefined()
		return 'Node'

class DecisionTreeClassifier( classificationMethod.ClassificationMethod ) :
	def __init__( self , legalLabels ) :
		self.guess = None
		self.type = "decisiontree"

	def train( self , data , labels , args ) :
		"""
		Learn the tree model
		"""
		self.maxdepth = int( args[ 'maxdepth' ] )
		self.metric = args[ 'metric' ]
		self.numrows = len( data )
		self.numcolumns = len( data[ 0 ] )
		# l = self.getMostFrequentLabel(labels)
		# self.tree = DecisionNode(self.bestSplit(data, labels), None, l, None, None)
		self.tree = self.buildTree( data , labels )

	def buildTree( self , data , labels , depth = 0 ) :
		""" Recursive function to learn the tree model """
		if self.isLeaf(data, labels, depth):
		  return DecisionNode(None, None, self.getMostFrequentLabel(labels), None, None)
		
		splitter = self.bestSplit(data, labels) # Atributo que divide o conjunto de dados
		sets = self.divideSet(data, labels, splitter) # Conjunto de dados de cada filho e suas respectivas labels
		
		leftChild = self.buildTree(sets[0], sets[1], depth + 1)
		rightChild = self.buildTree(sets[2], sets[3], depth + 1)
		
		return DecisionNode(splitter, 0, None, leftChild, rightChild)
		
		# util.raiseNotDefined()

	def isLeaf( self , data , labels , depth ) :
		""" Verify stop conditions (whether to split is necessary) """
		length = len(data)
		if length == 0: # Verifica se o conjutno de dados está vazio
		  return True
		
		if labels.count(0) == length or labels.count(1) == length: # Verifica se o conjunto de dados é puro
		  return True
		
		if data.count(data[0]) == length: # Verifica se todas as entradas do conjunto de dados são iguais
		  return True
		
		if depth == self.maxdepth: # Verifica se atingiu a pronfudidade máxima
		  return True
		
		return False
		
		# util.raiseNotDefined()

	def getMostFrequentLabel( self , labels ) :
	  if labels.count(0) > labels.count(1):
	    return 0
	  else:
	    return 1
		# util.raiseNotDefined()

	def bestSplit( self , data , labels ) :
		""" Get the best variable to split the dataset using the metric function """
		bestAtribute = None
		m = 10.0
		
		# Verifica todos os atributos
		for a in range(self.numcolumns):
		  sets = self.divideSet(data, labels, a) # Recebe os conjuntos de labels
		  
		  labels0 = sets[1]
		  labels1 = sets[3]
		  
		  m0 = self.metric(labels0)
		  m1 = self.metric(labels1)
		  
		  aux = m0 * (float(len(labels0)) / len(labels)) + m1 * (float(len(labels1)) / len(labels))
		  if aux < m:
		    bestAtribute = a
		    m = aux
		
		return bestAtribute
		# util.raiseNotDefined()

	def divideSet( self , data , label , variable ) :
		"""
		Given a variable, split the data set and labels in two sets.
		One data set with instances having variable as 0 and the
		other with instances having variable as 1
		"""
		data0 = []
		labels0 = []
		
		data1 = []
		labels1 = []
		
		for r in range(len(data)):
		  if data[r][variable] == 0:
		    data0.append(data[r])
		    labels0.append(label[r])
		  else:
		    data1.append(data[r])
		    labels1.append(label[r])
		
		return [data0, labels0, data1, labels1]
		# util.raiseNotDefined()

	def classify( self , testData ) :
		"""
		Classify all test data using the learned tree model
		"""
		labels = []
		
		for i in range(len(testData)):
		  node = self.tree
		  
		  # Percorre a árvore de acordo com os atributos até chegar na folha
		  while node.label == None:
		    a = node.column
		    if testData[i][a] == 0:
		      node = node.leftchild
		    else:
		      node = node.rightchild
		  
		  labels.append(node.label)
		
		return labels
		
		# util.raiseNotDefined()
