#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
A module to calculate significance and plot possible mass distribution
'''

__author__ = "Yousen Zhang"

import os
import ctypes
import ROOT as r

def setup(path=''):
  '''
  Include necessary files
  '''
  if len(path) > 0:
    if not path[-1] == '/':
      path = path + '/'
  incTop = os.path.expandvars('${OPENHF2020TOP}/Utilities')
  r.gInterpreter.ProcessLine('#include "%s/Ana/Common.h"' % incTop)
  r.gInterpreter.ProcessLine('#include "%smyAna.h"' % path)
  return

def scaleHistByYields(datafile, dataspec, genfile, genhist, yGenMin, yGenMax):
  '''
  Return a scale histogram.
  Users are reponsible for deleting return value
  '''
  h3 = r.TH3D('h3scale', ';y;pT(GeV);mass(GeV)',
      r.ana.nyAbs, r.ana.yAbsMin, r.ana.yAbsMax,
      r.ana.npt, r.ana.ptMin, r.ana.ptMax,
      r.ana.nmass, r.ana.massMin, r.ana.massMax)

  # spectrum
  fspectrum = r.TFile(datafile, 'READ')
  spectrum = fspectrum.Get(dataspec)
  if spectrum is None:
    print ('Null spectrum')
  ptEdges = []
  nBinsSpec = spectrum.GetNbinsX()
  for ibin in range(0, nBinsSpec+1):
    ptEdges.append(spectrum.GetBinLowEdge(ibin+1))

# generated spectrum
  fGen = r.TFile(genfile, 'READ')
  hGenYVsPt = fGen.Get(genhist)
  if hGenYVsPt is None:
    print ('Null generated spectrum')

  yGenBinMin = hGenYVsPt.GetYaxis().FindBin(yGenMin)
  # I know it will be at low edge of next bin I want
  yGenBinMax = hGenYVsPt.GetYaxis().FindBin(yGenMax) - 1
  hGenPt = hGenYVsPt.ProjectionX('hGenPt', yGenBinMin, yGenBinMax)

  ratios = []
  ratiosErr = []
  for ibin in range(len(ptEdges)-1):
    ptGenBinMin = hGenPt.GetXaxis().FindBin(ptEdges[ibin])
    # I know it will be at low edge of next bin I want
    ptGenBinMax = hGenPt.GetXaxis().FindBin(ptEdges[ibin+1]) - 1
    ratio = spectrum.GetBinContent(ibin+1) /hGenPt.Integral(ptGenBinMin, ptGenBinMax)
    ratio = ratio * spectrum.GetBinWidth(ibin+1) * r.ana.BR * 2
    ratios.append( ratio )
    # temp use
    error = spectrum.GetBinError(ibin+1) /hGenPt.Integral(ptGenBinMin, ptGenBinMax)
    #ratiosErr.append(error)
    ratiosErr.append(1e-15)

  for ipt in range(r.ana.npt):
    ibin = 0
    pt = h3.GetYaxis().GetBinCenter(ipt+1)
    for i in range(len(ptEdges)-1):
      if pt<ptEdges[i+1] and pt>ptEdges[i]:
        ibin = i
    scale = ratios[ibin]
    error = ratiosErr[ibin]
    for iy in range(r.ana.nyAbs):
      for im in range(r.ana.nmass):
        h3.SetBinContent(iy+1, ipt+1, im+1, scale)
        h3.SetBinError(iy+1, ipt+1, im+1, error)

  spectrum.Delete()
  hGenPt.Delete()
  fspectrum.Close()
  fGen.Close()

  #h3.SetDirectory(r.gROOT) # detach h3
  return h3

def scaleHist(TAA, datafile, dataspec, genfile, genhist, yGenMin, yGenMax):
  '''
  Return a scale histogram.
  Users are reponsible for deleting return value
  '''
  h3 = r.TH3D('h3scale', ';y;pT(GeV);mass(GeV)',
      r.ana.nyAbs, r.ana.yAbsMin, r.ana.yAbsMax,
      r.ana.npt, r.ana.ptMin, r.ana.ptMax,
      r.ana.nmass, r.ana.massMin, r.ana.massMax)

  # spectrum
  fspectrum = r.TFile(datafile, 'READ')
  spectrum = fspectrum.Get(dataspec)
  if spectrum is None:
    print ('Null spectrum')
  ptEdges = []
  nBinsSpec = spectrum.GetNbinsX()
  for ibin in range(0, nBinsSpec+1):
    ptEdges.append(spectrum.GetBinLowEdge(ibin+1))

# generated spectrum
  fGen = r.TFile(genfile, 'READ')
  hGenYVsPt = fGen.Get(genhist)
  if hGenYVsPt is None:
    print ('Null generated spectrum')

  yGenBinMin = hGenYVsPt.GetYaxis().FindBin(yGenMin)
  # I know it will be at low edge of next bin I want
  yGenBinMax = hGenYVsPt.GetYaxis().FindBin(yGenMax) - 1
  hGenPt = hGenYVsPt.ProjectionX('hGenPt', yGenBinMin, yGenBinMax)

  ratios = []
  ratiosErr = []
  for ibin in range(len(ptEdges)-1):
    ptGenBinMin = hGenPt.GetXaxis().FindBin(ptEdges[ibin])
    # I know it will be at low edge of next bin I want
    ptGenBinMax = hGenPt.GetXaxis().FindBin(ptEdges[ibin+1]) - 1
    ratio = spectrum.GetBinContent(ibin+1) /hGenPt.Integral(ptGenBinMin, ptGenBinMax)
    ratio = ratio * TAA * r.ana.pbOvermb * spectrum.GetBinWidth(ibin+1) * r.ana.BR * 2
    ratios.append( ratio )
    # temp use
    error = spectrum.GetBinError(ibin+1) /hGenPt.Integral(ptGenBinMin, ptGenBinMax)
    #ratiosErr.append(error)
    ratiosErr.append(1e-15)

  for ipt in range(r.ana.npt):
    ibin = 0
    pt = h3.GetYaxis().GetBinCenter(ipt+1)
    for i in range(len(ptEdges)-1):
      if pt<ptEdges[i+1] and pt>ptEdges[i]:
        ibin = i
    scale = ratios[ibin]
    error = ratiosErr[ibin]
    for iy in range(r.ana.nyAbs):
      for im in range(r.ana.nmass):
        h3.SetBinContent(iy+1, ipt+1, im+1, scale)
        h3.SetBinError(iy+1, ipt+1, im+1, error)

  spectrum.Delete()
  hGenPt.Delete()
  fspectrum.Close()
  fGen.Close()

  #h3.SetDirectory(r.gROOT) # detach h3
  return h3

def getMassVsPtVsY(fname, mtd, postfix=''):
  f = r.TFile(fname, 'READ')
  s = 'Mtd' if mtd else ''
  s = s+postfix
  h3 = f.Get('hMassVsPtVsY%s' % s)
  if h3 == None:
    print ('Null hMassVsPtVsY%s' % s)
    return None
  h3.SetDirectory(r.gROOT) # detach h3 from f
  return h3

def getSignal(fname, mtd, h3scale, scale=True, cent1=1, cent2=200):
  h3signal = getMassVsPtVsY(fname, mtd)
  h = None
  if h3scale != None:
    #h = r.ana.multiply('TH3D')(h3signal, h3scale)
    h = r.ana.multiply['TH3D'](h3signal, h3scale)
    h.Scale(r.ana.evts_sim_MB *(cent2-cent1+1)/200)
# do not know how to use * for TH3D in PyROOT
  else:
    h = h3signal
    if scale:
      print ('You want to scale the histogram but the scaling is None')
  postfix = '_scalesignal' if scale else '_signal'
  h.SetName(h3signal.GetName()+postfix)
  return h

def getBackground(fname, mtd, scale=True, cent1=1, cent2=200, poststr=""):
  h = getMassVsPtVsY(fname, mtd, poststr)
  postfix = '_scalebackground' if scale else '_background'
  h.SetName(h.GetName()+postfix)
  scalefactor = 1
  #print h.Integral()
  if scale:
    f = r.TFile(fname, 'READ')
    hCent = f.Get('hCent')
    nEvts = hCent.Integral(cent1, cent2)
    scalefactor = r.ana.evts_sim_MB/nEvts * (cent2-cent1+1)/200.
    #print "scalefactor", scalefactor, "nEvets", nEvts, "centrality", (cent2-cent1+1)/200., "sim MB", r.ana.evts_sim_MB
    h.Scale(scalefactor)
  return h, scalefactor

def getCounts(h3, yMin, yMax, pTMin, pTMax, massMin, massMax):

  yBinMin = h3.GetXaxis().FindBin(yMin)
  yBinMax = h3.GetXaxis().FindBin(yMax) - 1

  pTBinMin = h3.GetYaxis().FindBin(pTMin)
  pTBinMax = h3.GetYaxis().FindBin(pTMax) - 1

  massBinMin = h3.GetZaxis().FindBin(massMin)
  massBinMax = h3.GetZaxis().FindBin(massMax)

  #print yBinMin, yBinMax, pTBinMin, pTBinMax, massBinMin, massBinMax

  err = ctypes.c_double(0.)
  yields = h3.IntegralAndError(yBinMin, yBinMax, pTBinMin, pTBinMax, massBinMin, massBinMax, err)
  #print yields, err.value
  return (yields, err.value)

if __name__ == '__main__':
  print ('Loaded calculator')
