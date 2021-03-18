#!/usr/bin/env python
import numpy as np
import pandas as pd

def calSigY2(df, minPt, maxPt):
  S = np.sum(df["S0"][minPt:maxPt]) + np.sum(df["S1"][minPt:maxPt])
  SErr = np.sqrt( np.sum(df["SErr0"][minPt:maxPt])**2 + np.sum(df["SErr1"][minPt:maxPt])**2 )
  B = np.sum(df["B0"][minPt:maxPt]) + np.sum(df["B1"][minPt:maxPt])
  BErr = np.sqrt( np.sum(df["BErr0"][minPt:maxPt])**2 + np.sum(df["BErr1"][minPt:maxPt])**2 )
  return S/np.sqrt(S + B)

def calSigY1p2(df, minPt, maxPt):
  S = np.sum(df["S0"][minPt:maxPt])
  SErr = np.sqrt( np.sum(df["SErr0"][minPt:maxPt])**2 )
  B = np.sum(df["B0"][minPt:maxPt])
  BErr = np.sqrt( np.sum(df["BErr0"][minPt:maxPt])**2 )
  return S/np.sqrt(S + B)
def insertPt(df):
  pt = ['0-0.5', '0.5-1.0', '1.0-2.0', '2.0-3.0', '3.0-4.0', '4.0-5.0',
        '5.0-6.0', '6.0-8.0', '8.0-10.0', '10.0-12.5', '12.5-15.0', '15-20',
        '20-25', '25-30', '30-40']
  df.insert(0, "pT", pt)

wMTD = pd.read_csv('SBMB.txt')
insertPt(wMTD)
woMTD = pd.read_csv('SBMBWoMTD.txt')
insertPt(woMTD)

#print(wMTD["S0"][5:9])
#print(wMTD["SErr0"][5:9])
#print(wMTD["B0"][5:9])
#print(wMTD["BErr0"][5:9])
print('D0 yields projection with MTD for jet-D, 0, 1, 2 means |y| 0-1, 1-2, 2-3')
print(wMTD)
print
print('D0 yields projection without MTD for jet-D, 0, 1, 2 means |y| 0-1, 1-2, 2-3')
print(woMTD)

# calculate wMTD
print 'Jet-D'
print 'D significance wMTD, from 4-20GeV, |y|<2', calSigY2(wMTD, 5, 12)
print 'D significance wMTD, from 20-30GeV, |y|<2', calSigY2(wMTD, 12, 14)

# calculate woMTD
print 'D significance woMTD, from 4-20GeV, |y|<2', calSigY2(woMTD, 5, 12)
print 'D significance woMTD, from 20-30GeV, |y|<2', calSigY2(woMTD, 12, 14)
print

wMTDPhoto = pd.read_csv('SBMBPhoto.txt')
insertPt(wMTDPhoto)
woMTDPhoto = pd.read_csv('SBMBWoMTDPhoto.txt')
insertPt(woMTDPhoto)

print('D0 yields projection with MTD for photo-D, 0 means |y| 0-1.2')
print(wMTDPhoto)
print
print('D0 yields projection without MTD for photo-D, 0 means |y| 0-1.2')
print(woMTDPhoto)

print 'Photo-D'
print 'D significance wMTD, from 5-30GeV, |y|<1.2', calSigY1p2(wMTDPhoto, 6, 14)
print 'D significance woMTD, from 5-30GeV, |y|<1.2', calSigY1p2(woMTDPhoto, 6, 14)

print 'Photo-D'
print 'D significance wMTD, from 4-30GeV, |y|<1.2', calSigY1p2(wMTDPhoto, 5, 14)
print 'D significance woMTD, from 4-30GeV, |y|<1.2', calSigY1p2(woMTDPhoto, 5, 14)

print 'Photo-D'
print 'D significance wMTD, from 4-10GeV, |y|<1.2', calSigY1p2(wMTDPhoto, 5, 9)
print 'D significance woMTD, from 4-10GeV, |y|<1.2', calSigY1p2(woMTDPhoto, 5, 9)

print 'Photo-D'
print 'D significance wMTD, from 2-6GeV, |y|<1.2', calSigY1p2(wMTDPhoto, 3, 7)
print 'D significance woMTD, from 2-6GeV, |y|<1.2', calSigY1p2(woMTDPhoto, 3, 7)

print 'Photo-D'
print 'D significance wMTD, from 2-10GeV, |y|<1.2', calSigY1p2(wMTDPhoto, 3, 9)
print 'D significance woMTD, from 2-10GeV, |y|<1.2', calSigY1p2(woMTDPhoto, 3, 9)

