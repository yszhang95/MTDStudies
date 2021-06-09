#!/usr/bin/env python
import sys
sys.path.append('../../Utilities/Ana/')

import math
import calculator
import ROOT as r
import numpy as np

from array import array

calculator.setup()

def calSig(yMin, yMax, pTMin, pTMax, topoMin, topoMax, hsignal, hbackground):
  (s, serr) = calculator.getCounts(hsignal, yMin, yMax, pTMin, pTMax, topoMin, topoMax)
  (b, berr) = calculator.getCounts(hbackground, yMin, yMax, pTMin, pTMax, topoMin, topoMax)
  #print hsignal.GetName(), topoMin, topoMax, pTMin, pTMax, s, s/serr, b, berr
  sig = s/math.sqrt(s+b)
  sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  return (s, serr, b, berr, sig, sigErr)

yMinEdges = [0.]
yMaxEdges = [1.]
ptMinEdges = { 'pT2p95' : [3., 4., 5., 6., 8.], 'pT0to3p05' : [2.]}
ptMaxEdges = { 'pT2p95' : [4., 5., 6., 8., 10.], 'pT0to3p05' : [3.]}

topos = ['DLDs', 'AngleDs', 'PionPt', 'PosKPt', 'NegKPt', 'PhiMass', 'VtxProb', 'AnglePhi', 'VtxProb']
cutmins = {'DLDs' : [ i for i in np.arange(0, 4, 0.2)],
    'AngleDs' : [0] * len(np.arange(0.04, 0.46, 0.02)),
    'PionPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PosKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'NegKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PhiMass' : [0] * len(np.arange(0.001, 0.01, 0.001)),
    'AnglePhi' : [0] * len(np.arange(1, 3.14, 0.02)),
    'VtxProb' : np.arange(0.0, 0.5, 0.01).tolist()
    }
cutmaxs = {'DLDs' : [10000]*len(np.arange(0, 3.8, 0.2).tolist()),
    'AngleDs' : np.arange(0.04, 0.46, 0.02).tolist(),
    'PionPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    'PosKPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    'NegKPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    'AnglePhi' : np.arange(1, 3.14, 0.02),
    'PhiMass' : np.arange(0.001, 0.01, 0.001).tolist(),
    'VtxProb' : [1] * len(np.arange(0.0, 0.5, 0.01).tolist())
    }

cuts = {
    'DLDs' : [ i for i in np.arange(0, 4, 0.2)],
    'AngleDs' : np.arange(0.04, 0.46, 0.02).tolist(),
    'PionPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PosKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'NegKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PhiMass' : np.arange(0.001, 0.01, 0.001).tolist(),
    'AnglePhi' : np.arange(1., 3.14, 0.02),
    'VtxProb' : np.arange(0.0, 0.5, 0.01).tolist()
    }

sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'

bfiles = {
 'pT2p95'  : [ 'output/DsHydJets_pT2p95_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'],
 'pT0to3p05' : [ 'output/DsHydJets_pT0to3p05_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root' ]
  }

#h3scale = calculator.scaleHist(5.6, 'CMS-D0.root', 'Table 3/Hist1D_y1', sfile, 'hGenYVsPt', -1, 1.)
h3scale = calculator.scaleHistByYields('Alice-D0.root', 'Table 1/Hist1D_y1', sfile, 'hGenYVsPt', -0.5, 0.5)

fout = r.TFile('sigScanNew.root', "recreate")

fs = r.TFile(sfile)
# get all signal out
hs = {}
for v in topos:
  hs[v] = fs.Get('h%sVsDsPtVsDsY' % v)
for kf, vf in bfiles.items():
  for f,yMin,yMax in zip(vf, yMinEdges, yMaxEdges):
    fb = r.TFile(f)
    hCent = fb.Get("hCent")
    hb = {}
    for v in topos:
      hb[v] = fb.Get('h%sVsDsPtVsDsY' % v)
      for pTMin, pTMax in zip(ptMinEdges[kf], ptMaxEdges[kf]):
        ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2.)
        scale = h3scale.GetBinContent(1, ibin, 1)
        print "scale", scale
        cutValue, cutErr, sigValue, sigErr = array('d'), array('d'), array('d'), array('d')
        for cutmin, cutmax, cut in zip(cutmins[v], cutmaxs[v], cuts[v]):
          (_, _, _, _, sig, e) = calSig(yMin, yMax, pTMin, pTMax, cutmin, cutmax, hs[v], hb[v])
          cutValue.append(cut)
          cutErr.append(0)
          #sigValue.append(sig * scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.Integral(1,20)) )
          sigValue.append(sig * scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.Integral()) )
          sigErr.append(e * scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries()) )
        gr = r.TGraphErrors(len(cutValue), cutValue, sigValue, cutErr, sigErr)
        gr.SetName('gSigVs%s_pT%gto%g_y%gto%g' % (v, pTMin, pTMax, yMin, yMax))
        fout.cd()
        gr.Write()
