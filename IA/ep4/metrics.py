# -*- coding: utf-8 -*-

# Nome: Vítor Kei Taira Tamada
# NUSP: 8516250

import sys
import math
import util

def gini( labels ) :
	l = 0.0
	if len(labels) == 0:
	  return 1
	l += math.pow(float(labels.count(0)) / len(labels), 2)
	l += math.pow(float(labels.count(1)) / len(labels), 2)
	
	return 1 - l
	
	# util.raiseNotDefined()

def error( labels ) :
  if len(labels) == 0:
    return 1

  l0 = float(labels.count(0)) / len(labels)
  l1 = float(labels.count(1)) / len(labels)
  
  if l0 > l1:
    return 1 - l0
  else:
    return 1 - l1

	# util.raiseNotDefined()

def entropy( labels ) :
  if len(labels) == 0:
    return 0
  l = 0.0
  p0 = float(labels.count(0)) / len(labels)
  if p0 != 0:
    l += p0 * math.log(p0)
  p1 = float(labels.count(1)) / len(labels)
  if p1 != 0:
    l += p1 * math.log(p1)
  
  return -l
  
	# util.raiseNotDefined()
