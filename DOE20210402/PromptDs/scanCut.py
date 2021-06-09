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
  sig = s/math.sqrt(s+b)
  sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  #print ss, pTMin, pTMax, sig, sigErr, s, s/serr, b, b/berr
  return (s, serr, b, berr, sig, sigErr)

yMinEdges = [0., 1, 2]
yMaxEdges = [1., 2, 3]
ptMinEdges = { 'pT2p95' : [3., 4., 5., 6., 8.], 'pT0to3p05' : [2.]}
ptMaxEdges = { 'pT2p95' : [4., 5., 6., 8., 10.], 'pT0to3p05' : [3.]}

topos = ['DLDs', 'AngleDs', 'PionPt', 'PosKPt', 'NegKPt']
cutmins = {'DLDs' : [ i for i in np.arange(0, 4, 0.2)],
    'AngleDs' : [0] * len(np.arange(0.04, 0.46, 0.02)),
    'PionPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PosKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'NegKPt' : np.arange(0.4, 1.0, 0.1).tolist()
    }
cutmaxs = {'DLDs' : [10000]*len(np.arange(0, 4, 0.2).tolist()),
    'AngleDs' : np.arange(0.04, 0.46, 0.02).tolist(),
    'PionPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    'PosKPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    'NegKPt' : [10000] * len(np.arange(0.4, 1.0, 0.1)),
    }

cuts = {
    'DLDs' : [ i for i in np.arange(0, 4, 0.2)],
    'AngleDs' : np.arange(0.04, 0.46, 0.02).tolist(),
    'PionPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'PosKPt' : np.arange(0.4, 1.0, 0.1).tolist(),
    'NegKPt' : np.arange(0.4, 1.0, 0.1).tolist()
    }

sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'

bfiles = { 'pT2p95'  : [ 'output/DsHydJets_pT2p95_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'],
  'pT0to3p05' : ['output/DsHydJets_pT0to3p05_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root']
  }

h3scale = calculator.scaleHist(5.6, 'CMS-D0.root', 'Table 3/Hist1D_y1', sfile, 'hGenYVsPt', -1, 1.)

fout = r.TFile('sigScan.root', "recreate")

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
        cutValue, cutErr, sigValue, sigErr = array('d'), array('d'), array('d'), array('d')
        for cutmin, cutmax, cut in zip(cutmins[v], cutmaxs[v], cuts[v]):
          (_, _, _, _, sig, e) = calSig(yMin, yMax, pTMin, pTMax, cutmin, cutmax, hs[v], hb[v])
          cutValue.append(cut)
          cutErr.append(0)
          sigValue.append(sig * scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries()) )
          sigErr.append(e * scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries()) )
        gr = r.TGraphErrors(len(cutValue), cutValue, sigValue, cutErr, sigErr)
        gr.SetName('gSigVs%s_pT%gto%g_y%gto%g' % (v, pTMin, pTMax, yMin, yMax))
        fout.cd()
        gr.Write()

# Pion pT
#    print "Pion pT"
#    for cut in [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 1, hs["hPionPtVsDsPtVsDsY"], hb['hPionPtVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
#
#    print "Positive Kaon pT"
#    for cut in [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 1, hs["hPosKPtVsDsPtVsDsY"], hb['hPosKPtVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
#
#    print "Negative Kaon pT"
#    for cut in [0.4, 0.5, 0.6, 0.7, 0.8, 0.9]:
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 1, hs["hNegKPtVsDsPtVsDsY"], hb['hNegKPtVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
#    print "Ds DL"

#    for cut in np.arange(0., 3., 0.2):
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 1000, hs["hDLDsVsDsPtVsDsY"], hb['hDLDsVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
#    for cut in np.arange(0., 3., 0.2):
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 10000, hs["hDLPhiVsDsPtVsDsY"], hb['hDLPhiVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())

#    for cut in np.arange(0.,0.45, 0.05):
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, 0, cut, hs["hAngleDsVsDsPtVsDsY"], hb['hAngleDsVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
#    for cut in np.arange(0.5, 1, 0.05):
#      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2)
#      scale = h3scale.GetBinContent(1, ibin, 1)
#      (_, _, _, _, sig, _) = calSig(yMin, yMax, pTMin, pTMax, cut, 100, hs["hAnglePhiVsDsPtVsDsY"], hb['hAnglePhiVsDsPtVsDsY'])
#      print pTMin, pTMax, cut, 0.35*sig*scale*r.TMath.Sqrt(r.ana.evts_sim_MB * hCent.GetEntries())
