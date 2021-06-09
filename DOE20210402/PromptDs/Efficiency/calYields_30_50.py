#!/usr/bin/env python
import sys
sys.path.append('../../../Utilities/Ana/')

import math
import calculator
import ROOT as r

from array import array

def calSig(yMin, yMax, pTMin, pTMax, massMin, massMax, useMTD, sFile, bFile):
  calculator.setup()
  hsignal = calculator.getSignal(sFile, useMTD, h3scale, True, 61, 100)
  #hsignal.Scale(0.35) # Ds to D0 ratio
  (hbackground, scale) = calculator.getBackground(bFile, useMTD, True, 61, 100, '_30_50')
  (s, serr) = calculator.getCounts(hsignal, yMin, yMax, pTMin, pTMax, massMin, massMax)
  (b, berr) = calculator.getCounts(hbackground, yMin, yMax, pTMin, pTMax, massMin, massMax)
  sig = s/math.sqrt(s+b)
  sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  hsignal.Delete()
  hbackground.Delete()
  ss = "wMTD" if useMTD else 'woMTD'
  #print ss, pTMin, pTMax, sig, sigErr, s, s/serr, b, b/berr
  return (sig, sigErr, s, serr, b, berr)

calculator.setup()
sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'
h3scale = calculator.scaleHistByYields('Alice-D0.root', 'Table 5/Hist1D_y1', sfile, 'hGenYVsPt', -0.5, 0.5)

bfiles = { 'pT2p95'  : [ 'output/DsHydJets_pT2p95_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'],
  'pT0to3p05' : ['output/DsHydJets_pT0to3p05_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root']
  }

yRanges = ['0to1', '1to2', '2to3']
yMinEdges = [0., 1, 2]
yMaxEdges = [1., 2, 3]
ptMinEdges = { 'pT2p95' : [3., 4., 5., 6., 7., 8.], 'pT0to3p05' : [2.]}
ptMaxEdges = { 'pT2p95' : [4., 5., 6., 7., 8., 10.], 'pT0to3p05' : [3.]}

sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'

#print "MTD", "pTMin", "pTMax", "sig", "sigErr", "s", "s/sErr", "b", "b/bErr"
pTValue = {}
pTErr = {}
sigValue = { 'MTD' : {}, 'noMTD' : {}}
sigErr = { 'MTD' : {}, 'noMTD' : {}}
sValue = { 'MTD' : {}, 'noMTD' : {}}
sErr = { 'MTD' : {}, 'noMTD' : {}}
bValue = { 'MTD' : {}, 'noMTD' : {}}
bErr = { 'MTD' : {}, 'noMTD' : {}}

for kf, vf in bfiles.items():
  pTValue[kf] = {}
  pTErr[kf] = {}

  sigValue['MTD'][kf] = {}
  sigErr['MTD'][kf] = {}
  sigValue['noMTD'][kf] = {}
  sigErr['noMTD'][kf] = {}

  sValue['MTD'][kf] = {}
  sErr['MTD'][kf] = {}
  sValue['noMTD'][kf] = {}
  sErr['noMTD'][kf] = {}

  bValue['MTD'][kf] = {}
  bErr['MTD'][kf] = {}
  bValue['noMTD'][kf] = {}
  bErr['noMTD'][kf] = {}

  for f, yMin, yMax, yrange in zip(vf, yMinEdges, yMaxEdges, yRanges):
    pTValue[kf][yrange] = array('d')
    pTErr[kf][yrange] = array('d')

    sigValue['MTD'][kf][yrange] = array('d')
    sigErr['MTD'][kf][yrange] = array('d')
    sigValue['noMTD'][kf][yrange] = array('d')
    sigErr['noMTD'][kf][yrange] = array('d')

    sValue['MTD'][kf][yrange] = array('d')
    sErr['MTD'][kf][yrange] = array('d')
    sValue['noMTD'][kf][yrange] = array('d')
    sErr['noMTD'][kf][yrange] = array('d')

    bValue['MTD'][kf][yrange] = array('d')
    bErr['MTD'][kf][yrange] = array('d')
    bValue['noMTD'][kf][yrange] = array('d')
    bErr['noMTD'][kf][yrange] = array('d')

    for pTMin, pTMax in zip(ptMinEdges[kf], ptMaxEdges[kf]):
      masslw = 1.96
      massup = 1.979
      (sig, err, s, serr, b, berr) = calSig(yMin, yMax, pTMin, pTMax, masslw, massup, True, sfile, f)
      (sigNoMTD, errNoMTD, sNoMTD, serrNoMTD, bNoMTD, berrNoMTD) = calSig(yMin, yMax, pTMin, pTMax, masslw, massup, False, sfile, f)
      pTValue[kf][yrange].append((pTMax+pTMin)/2.)
      pTErr[kf][yrange].append((pTMax-pTMin)/2.)

      sigValue['MTD'][kf][yrange].append(sig)
      sigErr['MTD'][kf][yrange].append(err)
      sigValue['noMTD'][kf][yrange].append(sigNoMTD)
      sigErr['noMTD'][kf][yrange].append(errNoMTD)

      sValue['MTD'][kf][yrange].append(s)
      sErr['MTD'][kf][yrange].append(serr)
      sValue['noMTD'][kf][yrange].append(sNoMTD)
      sErr['noMTD'][kf][yrange].append(serrNoMTD)

      bValue['MTD'][kf][yrange].append(b)
      bErr['MTD'][kf][yrange].append(berr)
      bValue['noMTD'][kf][yrange].append(bNoMTD)
      bErr['noMTD'][kf][yrange].append(berrNoMTD)

ofile = r.TFile('sig_30_50.root', "recreate")
for yrange in yRanges:
  x = pTValue["pT0to3p05"][yrange] + pTValue["pT2p95"][yrange]
  xerr = pTErr["pT0to3p05"][yrange] + pTErr["pT2p95"][yrange]

  y = sigValue['MTD']["pT0to3p05"][yrange] + sigValue['MTD']["pT2p95"][yrange]
  yerr = sigErr['MTD']["pT0to3p05"][yrange] + sigErr['MTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gSigVsPt_%s' % yrange)
  gr.Write()

  y = sigValue['noMTD']["pT0to3p05"][yrange] + sigValue['noMTD']["pT2p95"][yrange]
  yerr = sigErr['noMTD']["pT0to3p05"][yrange] + sigErr['noMTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gSigVsPt_%s_noMTD' % yrange)
  gr.Write()

  y = sValue['MTD']["pT0to3p05"][yrange] + sValue['MTD']["pT2p95"][yrange]
  yerr = sErr['MTD']["pT0to3p05"][yrange] + sErr['MTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gSVsPt_%s' % yrange)
  gr.Write()
  y = sValue['noMTD']["pT0to3p05"][yrange] + sValue['noMTD']["pT2p95"][yrange]
  yerr = sErr['noMTD']["pT0to3p05"][yrange] + sErr['noMTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gSVsPt_%s_noMTD' % yrange)
  gr.Write()

  y = bValue['MTD']["pT0to3p05"][yrange] + bValue['MTD']["pT2p95"][yrange]
  yerr = bErr['MTD']["pT0to3p05"][yrange] + bErr['MTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gBVsPt_%s' % yrange)
  gr.Write()

  y = bValue['noMTD']["pT0to3p05"][yrange] + bValue['noMTD']["pT2p95"][yrange]
  yerr = bErr['noMTD']["pT0to3p05"][yrange] + bErr['noMTD']["pT2p95"][yrange]
  gr = r.TGraphErrors(len(x), x, y, xerr, yerr)
  gr.SetName('gBVsPt_%s_noMTD' % yrange)
  gr.Write()

h3scale.Delete()
