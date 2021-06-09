#!/usr/bin/env python
import sys
sys.path.append('../../../Utilities/Ana/')

import math
import calculator
import ROOT as r
import numpy as np

import ctypes

from array import array

calculator.setup()

def getCounts(h3, yMin, yMax, pTMin, pTMax, massMin, massMax):

  yBinMin = h3.GetXaxis().FindBin(yMin)
  yBinMax = h3.GetXaxis().FindBin(yMax) - 1

  pTBinMin = h3.GetYaxis().FindBin(pTMin)
  pTBinMax = h3.GetYaxis().FindBin(pTMax)

  massBinMin = h3.GetZaxis().FindBin(massMin)
  massBinMax = h3.GetZaxis().FindBin(massMax)

  err = ctypes.c_double(0.)
  yields = h3.IntegralAndError(yBinMin, yBinMax, pTBinMin, pTBinMax, massBinMin, massBinMax, err)
  return (yields, err.value)

def calSig(pTMin, pTMax, topoMin1, topoMax1, topoMin2, topoMax2, hsignal, hbackground, scales, scaleb):
  #print hsignal.GetEntries()
  #print hbackground.GetEntries()
  #print topoMin1, topoMax1
  #print topoMin2, topoMax2
  (s, serr) = getCounts(hsignal, pTMin, pTMax, topoMin1, topoMax1, topoMin2, topoMax2)
  (b, berr) = getCounts(hbackground, pTMin, pTMax, topoMin1, topoMax1, topoMin2, topoMax2)
  s = s * scales
  b = b * scaleb
  serr = serr * scales
  berr = berr * scaleb
  #print pTMin, pTMax, s, b
  sig = s/math.sqrt(s+b)
  sigErr = sig* math.sqrt( (serr/s - 0.5 * serr/(s+b)) **2 + ( 0.5*berr/(s+b) )**2 )
  #print ss, pTMin, pTMax, sig, sigErr, s, s/serr, b, b/berr
  return (s, serr, b, berr, sig, sigErr)

#yMinEdges = [0., 1, 2]
#yMaxEdges = [1., 2, 3]
yMinEdges = [0.]
yMaxEdges = [1.]
ptMinEdges = { 'pT2p95' : [3., 4., 5., 6., 8.], 'pT0to3p05' : [2.]}
ptMaxEdges = { 'pT2p95' : [4., 5., 6., 8., 10.], 'pT0to3p05' : [3.]}

topos = ['DLDs', 'AngleDs']
cutmins = {'DLDs' : [ i for i in np.arange(0, 3.5, 0.01)],
    'AngleDs' : [0] * len(np.arange(0.04, 0.46, 0.01))
    }
cutmaxs = {'DLDs' : [10000]*len(np.arange(0, 3.5, 0.01).tolist()),
    'AngleDs' : np.arange(0.04, 0.46, 0.01).tolist()
    }

cuts = {
    'DLDs' : [ i for i in np.arange(0, 3.5, 0.01)],
    'AngleDs' : np.arange(0.04, 0.46, 0.01).tolist()
    }

sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'

bfiles = { 'pT2p95'  : [ 'output/DsHydJets_pT2p95_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT2p95_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'],
  'pT0to3p05' : ['output/DsHydJets_pT0to3p05_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y0p95to2p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root',
  'output/DsHydJets_pT0to3p05_y1p95to3p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root']
  }

#h3scale = calculator.scaleHist(5.6, 'CMS-D0.root', 'Table 3/Hist1D_y1', sfile, 'hGenYVsPt', -1, 1.)
h3scale = calculator.scaleHistByYields('Alice-D0.root', 'Table 1/Hist1D_y1', sfile, 'hGenYVsPt', -0.5, 0.5)

fout = r.TFile('sigScan.root', "recreate")

fs = r.TFile(sfile)
# get all signal out
#hy =  { "y0to1" : [], "y1to2" : [], "y2to3" : [] }
hy =  { "y0to1" : [] }
hsy =  { "y0to1" : [] }
hby =  { "y0to1" : [] }

ofile = r.TFile("sigScan2D.root", "recreate")
for kf, vf in bfiles.items():
  iy = 0
  for f,yMin,yMax in zip(vf, yMinEdges, yMaxEdges):
    #print f
    hs = fs.Get('hDLVsAngleVsDsPt_y%d' % iy)
    fb = r.TFile(f)
    hCent = fb.Get("hCent")
    hb = fb.Get('hDLVsAngleVsDsPt_y%d' % iy)
    #print hb.GetEntries()
    ipt = 0
    for pTMin, pTMax in zip(ptMinEdges[kf], ptMaxEdges[kf]):
      print 'pTMin, pTMax', pTMin, pTMax
      print 'yMin, ymax', yMin, yMax
      ibin = h3scale.GetYaxis().FindBin((pTMin+pTMax)/2.)
      scale = h3scale.GetBinContent(1, ibin, 1)
      #print "scale", scale
      hy["y%.0fto%.0f" % (yMin, yMax)].append(r.TH2D("hSigVsDLVsAngle_pT%.0fto%.0f_y%.0fto%.0f" % (pTMin, pTMax, yMin, yMax), ";Angle;DL;Sig",\
        len(cuts['AngleDs']), 0.04, 0.46, len(cuts['DLDs']), 0, 4))
      hsy["y%.0fto%.0f" % (yMin, yMax)].append(r.TH2D("hSVsDLVsAngle_pT%.0fto%.0f_y%.0fto%.0f" % (pTMin, pTMax, yMin, yMax), ";Angle;DL;Sig",\
        len(cuts['AngleDs']), 0.04, 0.46, len(cuts['DLDs']), 0, 4))
      hby["y%.0fto%.0f" % (yMin, yMax)].append(r.TH2D("hBVsDLVsAngle_pT%.0fto%.0f_y%.0fto%.0f" % (pTMin, pTMax, yMin, yMax), ";Angle;DL;Sig",\
        len(cuts['AngleDs']), 0.04, 0.46, len(cuts['DLDs']), 0, 4))
      #print hy["y%.0fto%.0f" % (yMin, yMax)][ipt]
      for cutMin2, cutMax2, cut2 in zip(cutmins['DLDs'], cutmaxs['DLDs'], cuts['DLDs']):
        for cutMin1, cutMax1, cut1 in zip(cutmins['AngleDs'], cutmaxs['AngleDs'], cuts['AngleDs']):
          cut1Value, cut1Err, cut2Value, cut2Err, sigValue, sigErr = array('d'), array('d'), array('d'), array('d'), array('d'), array('d')
          (s, serr, b, berr, sig, e) = calSig(pTMin, pTMax, cutMin1, cutMax1, cutMin2, cutMax2, hs, hb, \
              scale* r.ana.evts_sim_MB *0.1, r.ana.evts_sim_MB *0.1/hCent.Integral(1,20))
          cut2Bin = hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetYaxis().FindBin(cut2)
          cut1Bin = hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetXaxis().FindBin(cut1)
          hy["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinContent(cut1Bin, cut2Bin, sig)
          hy["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinError(cut1Bin, cut2Bin, e)
          hsy["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinContent(cut1Bin, cut2Bin, s)
          hsy["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinError(cut1Bin, cut2Bin, serr)
          hby["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinContent(cut1Bin, cut2Bin, b)
          hby["y%.0fto%.0f" % (yMin, yMax)][ipt].SetBinError(cut1Bin, cut2Bin, berr)
        #gr = r.TGraphErrors(len(cutValue), cutValue, sigValue, cutErr, sigErr)
        #gr.SetName('gSigVs%s_pT%gto%g_y%gto%g' % (v, pTMin, pTMax, yMin, yMax))
        #fout.cd()
        #gr.Write()
      ofile.cd()
      hy["y%.0fto%.0f" % (yMin, yMax)][ipt].Write()
      maxBin = hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetMaximumBin()
      xbin, ybin, zbin = (r.Long(0), r.Long(1), r.Long(1))
      hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinXYZ(maxBin, xbin, ybin, zbin)
      print "sig", hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetMaximum(), hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinError(maxBin)
      print "S", hsy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinContent(maxBin), hsy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinError(maxBin)
      print "B", hby["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinContent(maxBin), hby["y%.0fto%.0f" % (yMin, yMax)][ipt].GetBinError(maxBin)
      print "angle < ", hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetXaxis().GetBinLowEdge(xbin)
      print "DL > ", hy["y%.0fto%.0f" % (yMin, yMax)][ipt].GetYaxis().GetBinLowEdge(ybin)
      ipt = ipt+1
    hy["y%.0fto%.0f" % (yMin, yMax)] = []
    hsy["y%.0fto%.0f" % (yMin, yMax)] = []
    hby["y%.0fto%.0f" % (yMin, yMax)] = []
    iy = iy+1
