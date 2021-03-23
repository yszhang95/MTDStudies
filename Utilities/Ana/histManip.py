#!/usr/bin/env python

# -*- coding: utf-8 -*-
'''
A module to combine mass distribution, depending on module calculator
'''

import calculator as cal

import ROOT as r

def getMassDistri(h3, yMin, yMax, pTMin, pTMax, postfix=''):
  yBinMin = h3.GetXaxis().FindBin(yMin)
  yBinMax = h3.GetXaxis().FindBin(yMax) - 1

  pTBinMin = h3.GetYaxis().FindBin(pTMin)
  pTBinMax = h3.GetYaxis().FindBin(pTMax) - 1

  h = h3.ProjectionZ("hMass_pT%.1f_%.1f_y%.1f_%.1f_%s" % (pTMin, pTMax, yMin, yMax, postfix))
  return h

def fitRawSignal(h, *pars):
  c = r.TCanvas("c", "", 500, 400)
  #signalShape = r.TF1("doubGaus", "gaus(0)+gaus(3)", r.ana.fitRangeLw, r.ana.fitRangeUp)
  signalShape = r.TF1("singGaus", "gaus(0)", r.ana.fitRangeLw, r.ana.fitRangeUp)
  i = 0
  for p in pars:
    signalShape.SetParameter(i, p)
    i = i+1
  h.Fit(signalShape, 'q')
  h.Fit(signalShape, "q")
  h.Fit(signalShape)
  h.Draw("E")
  c.Print("plots/%s.pdf" % h.GetName())
  c.Print("plots/%s.png" % h.GetName())
  signalShape.SetLineColor(r.kBlue)
  return signalShape
def generateSmoothSignal(h, f, n):
  c = r.TCanvas("c", "", 500, 400)
  print n
  h.FillRandom(f.GetName(), long(n))
  h.Draw("E")
  c.Print("plots/%s.pdf" % h.GetName())
  c.Print("plots/%s.png" % h.GetName())
def generateSmoothBackground(h, scale):
  tool = r.TSpectrum()
  c = r.TCanvas("c", "", 500, 400)
  b = tool.Background(h)
  h.Draw("E")
  b.Draw("ESAME")
  if scale != None:
    c.Print("plots/%s.pdf" % h.GetName())
    c.Print("plots/%s.png" % h.GetName())
    b.Scale(scale, "nosw2")
    b.SetName(b.GetName() + '_scaled')
  return b
def generateSumHist(hS, hB, pTMin, pTMax, yMin, yMax):
  h = hB.Clone('hMassAll_pT%.1f_%.1f_y%.1f_%.1f' % (pTMin, pTMax, yMin, yMax))
  h.Add(hS)
  c = r.TCanvas("c", "", 500, 400)
  h.Draw("E")
  c.Print("plots/%s.pdf" % h.GetName())
  c.Print("plots/%s.png" % h.GetName())
  return h

if __name__ == '__main__':
  print 'Loaded histManip.py'
