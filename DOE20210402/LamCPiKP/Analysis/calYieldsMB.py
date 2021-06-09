#!/usr/bin/env python
import sys
sys.path.append('../../../Utilities/Ana/')

import math
import calculator
import ROOT as r

from array import array

def calSig(yMin, yMax, pTMin, pTMax, massMin, massMax, useMTD, sFile, bFile):
  calculator.setup()
  hsignal = calculator.getSignal(sFile, useMTD, h3scale)
  (hbackground, scale) = calculator.getBackground(bFile, useMTD, True)
  (s, serr) = calculator.getCounts(hsignal, yMin, yMax, pTMin, pTMax, massMin, massMax)
  (b, berr) = calculator.getCounts(hbackground, yMin, yMax, pTMin, pTMax, massMin, massMax)
  b = b*2
  #print (pTMin, pTMax, s, serr, b, berr)
  sig = 0
  sigErr = 0
  if (s>1E-8):
    sig = s/math.sqrt(s+b)
    sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  hsignal.Delete()
  hbackground.Delete()
  ss = "wMTD" if useMTD else 'woMTD'
  print ('MTD', 'yMin', 'yMax', 'pTMin', 'pTMax', 'sig', 'sigErr', 's', 'serr', 'b', 'berr')
  print (ss, yMin, yMax, pTMin, pTMax, sig, sigErr, s, serr, b, berr, sep=', ')
  return (sig, sigErr, s, serr, b, berr)

calculator.setup()

# 1.8 RMS
sfile = 'output/LcPlusWPID_LamCPiKP.root'
bfile = 'output/HYDJETS_lamc_ana_mc_AllEntries_LamCPiKP.root'

#sfile = 'output/LcPlusWPID_AllEntries_LamCPiKP_1.00RMS.root'
#bfile = 'output/HYDJETS_lamc_ana_mc_AllEntries_LamCPiKP_1.00RMS.root'

#sfile = 'output/LcPlusWPID_AllEntries_LamCPiKP_1.20RMS.root'
#bfile = 'output/HYDJETS_lamc_ana_mc_AllEntries_LamCPiKP_1.20RMS.root'

#sfile = 'output/LcPlusWPID_AllEntries_LamCPiKP_1.50RMS.root'
#bfile = 'output/HYDJETS_lamc_ana_mc_AllEntries_LamCPiKP_1.50RMS.root'

#sfile = 'output/LcPlusWPID_AllEntries_LamCPiKP_1.80RMS.root'
#bfile = 'output/HYDJETS_lamc_ana_mc_AllEntries_LamCPiKP_1.80RMS.root'

fData = r.TFile('CMS-D0.root')
fHighPt = r.TFile("output/LcPlusWPID_highPt_lamc_ana_mc_AllEntries_LamCPiKP.root")
fLowPt  = r.TFile(sfile)
hData = fData.Get('Table 3/Hist1D_y1')
hGenYVsHighPt = fHighPt.Get('hGenYVsPt')
hGenYVsPt = fLowPt.Get('hGenYVsPt')
ybinlw = hGenYVsPt.GetXaxis().FindBin(-1.)
ybinup = hGenYVsPt.GetXaxis().FindBin(1.)-1
ptbin1 = hGenYVsPt.GetYaxis().FindBin(1.0)- 1
ptbin1p5 = hGenYVsPt.GetYaxis().FindBin(1.5)- 1
ptbin2 = hGenYVsPt.GetYaxis().FindBin(2.)
ptbin3 = hGenYVsPt.GetYaxis().FindBin(3.)- 1
scale = hData.GetBinContent(1)/hGenYVsHighPt.Integral(ybinlw, ybinup, ptbin2, ptbin3)
print ("scale", scale)
TAA = 5.6
scale = scale * TAA * r.ana.pbOvermb * hData.GetBinWidth(1) * r.ana.BR * 2
scale = scale * hGenYVsHighPt.Integral(ybinlw, ybinup, ptbin1+1, ptbin1p5)/hGenYVsPt.Integral(ybinlw, ybinup, ptbin1+1, ptbin1p5)
#print ("ratio", hGenYVsHighPt.Integral(ybinlw, ybinup, ptbin1+1, ptbin1p5)/hGenYVsPt.Integral(ybinlw, ybinup, ptbin1+1, ptbin1p5))
print ("scale", scale)

h3scale = r.TH3D("h3scale", "",\
    r.ana.ny, r.ana.yMin, r.ana.yMax, \
    r.ana.npt, r.ana.ptMin, r.ana.ptMax,\
    r.ana.nmass, r.ana.massMin, r.ana.massMax)
for iy in range(r.ana.ny+2):
  for ipt in range(r.ana.npt+2):
    for imass in range(r.ana.nmass+2):
      ibin = h3scale.GetBin(iy, ipt, imass)
      h3scale.SetBinContent(ibin, scale)
      h3scale.SetBinError(ibin, 0)

yRanges = ['0to1', '1to2', '2to3']
yMinEdges = [0., 1, 2]
yMaxEdges = [1., 2, 3]
nys = [0, 1, 2]
#yMinEdges = [1, 2]
#yMaxEdges = [2, 3]
#nys = [1, 2]
ptMinEdges = [0.]
ptMaxEdges = [1.]

for iy, yMin, yMax in zip(nys, yMinEdges, yMaxEdges):
  for pTMin, pTMax in zip(ptMinEdges, ptMaxEdges):
    masslw = r.ana.mass_lw[iy]
    massup = r.ana.mass_up[iy]
    (sig, err, s, serr, b, berr) = calSig(yMin, yMax, pTMin, pTMax, masslw, massup, True, sfile, bfile)
    (sig, err, s, serr, b, berr) = calSig(yMin, yMax, pTMin, pTMax, masslw, massup, False, sfile, bfile)
