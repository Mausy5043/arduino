#! /usr/bin/python
import numpy

C12=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(1,2))
C23=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(2,3))
C56=numpy.loadtxt('/tmp/testser.txt',delimiter=',',usecols=(5,6))
X = C12[:,1]
Y = C12[:,0]

print "TMP36 vs. DS18B20"
(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print X[-1], a*X[-1]+b, Y[-1]


X = C23[:,1]
Y = C23[:,0]
print "---"
print "DS18B20 vs. DHT22"
(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print X[-1], a*X[-1]+b, Y[-1]


X = C56[:,1]
Y = C56[:,0]
print "---"
print "DewPoint vs. DewPoint2"
(a,b)= numpy.polyfit(X,Y,1)
print a, b

print numpy.corrcoef(X,Y)[0,1]

print X[-1], a*X[-1]+b, Y[-1]
