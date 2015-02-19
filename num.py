#! /usr/bin/python
import numpy

C12=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(1,2))
C23=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(2,3))
C56=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(5,6))
X = C12[:,1]
Y = C12[:,0]

(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print a*X[0]+b, Y[0]


X = C23[:,1]
Y = C23[:,0]

(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print a*X[0]+b, Y[0]


X = C56[:,1]
Y = C56[:,0]

(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print a*X[0]+b, Y[0]
