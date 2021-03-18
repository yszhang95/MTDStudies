#!/usr/bin/env python
import ROOT as r
import math
print "ROOT has been imported"

# TDR parameters
z = 3 # m
r_min = 315 # mm
r_max = 1200 # mm

#print r.Math.PositionVector3D(r.Math.Cylindrical3D('double'))(r_max, z*1e3, 0).Eta()
#print r.Math.PositionVector3D(r.Math.Cylindrical3D('double'))(r_min, z*1e3, 0).Eta()

area = r.Math.Pi() * (r_max**2 - r_min**2)
print area

r_half = math.sqrt((r_max**2 + r_min**2)/2.)
print "radius corresponding to half area is: ", r_half
#print (r.Math.Pi() * (r_max ** 2 - r_half ** 2))

p3D_half = r.Math.PositionVector3D(r.Math.Cylindrical3D('double'))(r_half, z*1e3, 0)
print "Eta corresponding to r_half is :", p3D_half.Eta()

