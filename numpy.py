#! /usr/bin/python
import numpy

C=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(1,2))
X = C[:,1]
Y = C[:,0]
#Y1 = numpy.vstack([Y,numpy.ones(len(Y))]).T
#print numpy.linalg.lstsq(Y1,X)

(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print a*X[0]+b, Y[0]
