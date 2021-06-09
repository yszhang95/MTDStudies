#!/usr/bin/env python
import sys
sys.path.append("../../../Utilities/Ana/")

import ROOT as r
import calculator
import histManip
r.gROOT.SetBatch()

yMin = 0.
yMax = 1.
#pTMin = 2.
pTMin = 3.
#pTMax = 3.
pTMax = 4.

# set up
calculator.setup()

# Get 3D histogram

sfile = 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'
h3scale = calculator.scaleHist(r.ana.TAA0_100, 'CMS-D0.root', 'Table 3/Hist1D_y1', sfile, 'hGenYVsPt', -1, 1)
bfile = 'output/DsHydJets_pT0to3p05_y0to1p05_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root'

h3SMTD = calculator.getMassVsPtVsY(sfile, True)
h3S = calculator.getMassVsPtVsY(sfile, False)
h3BMTD = calculator.getMassVsPtVsY(bfile, True)
h3B = calculator.getMassVsPtVsY(bfile, False)

fb = r.TFile(bfile)
hCent = fb.Get("hCent")

# Get 1D histogram
h1SMTD = histManip.getMassDistri(h3SMTD, 0, 1, pTMin, pTMax, 'smtd')
h1BMTD = histManip.getMassDistri(h3BMTD, 0, 1, pTMin, pTMax, 'bmtd')
h1S = histManip.getMassDistri(h3S, 0, 1, pTMin, pTMax, 's')
h1B = histManip.getMassDistri(h3B, 0, 1, pTMin, pTMax, 'b')

# background

pt = (pTMin+pTMax)/2.
y = 0.5
ibin = h3scale.FindBin(y, pt, 1.987)
scale = h3scale.GetBinContent(ibin)*25e9

h1SMTD.Scale(scale, "nosw2")
h1S.Scale(scale, "nosw2")

h1BMTD.Scale(r.ana.evts_sim_MB/hCent.Integral(1, 200), "nosw2")
h1B.Scale(r.ana.evts_sim_MB/hCent.Integral(1, 200), "nosw2")

h1BMTD.Add(h1SMTD)
h1B.Add(h1S)

def combineSnB(hS, hB, rnd):
  '''
  Combine background and signal, background residual is assumed to be zero
  '''
  hSnB = r.TH1D(hS)
  hSnB.SetName(hS.GetName()+'_combined')
  for i in range(1, hB.GetNbinsX()+1):
    yerr = r.TMath.Sqrt(hB.GetBinContent(i) + hS.GetBinContent(i))
    y = float(sys.argv[1]) * hS.GetBinContent(i) + rnd.Gaus(0, yerr)
    hSnB.SetBinError(i, yerr)
    hSnB.SetBinContent(i, y)
  return hSnB

rnd = r.TRandom3()
rnd.SetSeed(0)

h1SnBMTD = combineSnB(h1SMTD, h1BMTD, rnd)
h1SnB = combineSnB(h1S, h1B, rnd)
h1SnBMTD = h1SnBMTD.Rebin(2)
h1SnB = h1SnB.Rebin(2)

ofile = r.TFile('DsSnBHists.root', "RECREATE")
h1SnBMTD.Write()
h1SnB.Write()

r.TGaxis.SetExponentOffset(-0.054, 0.01)
r.TGaxis.SetMaxDigits()
r.gStyle.SetPadTickX(1)
r.gStyle.SetPadTickY(1)
c = r.TCanvas("c", "", 1000, 400)
c.Divide(2, 1)
c.SetLeftMargin(0)
c.SetRightMargin(0)

line = r.TLine(r.ana.fitRangeLw, 0., r.ana.fitRangeUp, 0.)
line.SetLineStyle(2)
line.SetLineColor(r.kBlue)

tex =r.TLatex()
tex.SetTextSize(0.043)
tex.SetTextFont(42)
tex.SetTextAlign(22)

texCMS=r.TLatex()
texCMS.SetTextSize(0.05)

texMTD = r.TLatex()
texMTD.SetTextSize(0.045)
texMTD.SetTextFont(42)
texMTD.SetTextAlign(22)

texAA = r.TLatex()
texAA.SetTextSize(0.048)
texAA.SetTextFont(42)
texAA.SetTextAlign(31)

pad1 = c.cd(1)
pad1.SetLeftMargin(0.13)
pad1.SetRightMargin(0.04)
pad1.SetTopMargin(0.08)
pad1.SetBottomMargin(0.15)
h1SnBMTD.SetStats(0)
h1SnBMTD.SetTitle(";M_{K^{-}K^{+}#pi} (GeV);Events / 2 MeV")
h1SnBMTD.SetLineColor(39)
h1SnBMTD.SetMarkerStyle(20)
h1SnBMTD.SetMarkerSize(1.2)
h1SnBMTD.GetXaxis().SetTitleSize(0.05)
h1SnBMTD.GetYaxis().SetTitleSize(0.05)
h1SnBMTD.GetXaxis().SetRangeUser(r.ana.fitRangeLw, r.ana.fitRangeUp)
h1SnBMTD.GetXaxis().SetTitleOffset(1.1)
h1SnBMTD.GetYaxis().SetTitleOffset(1)
h1SnBMTD.GetXaxis().CenterTitle()
h1SnBMTD.GetYaxis().CenterTitle()
h1SnBMTD.SetMaximum(h1SnBMTD.GetMaximum() * 1.3)
h1SnBMTD.Draw("EP")
line.Draw()
tex.DrawLatexNDC(0.35, 0.8, "%.1f < p_{T} <%.1fGeV" % (pTMin, pTMax))
tex.DrawLatexNDC(0.35, 0.72, "|y| < 1")
texCMS.DrawLatexNDC(0.13, 0.94, "CMS Phase II #it{Simulation}")
texMTD.DrawLatexNDC(0.78, 0.76, "MTD")
texAA.DrawLatexNDC(0.96, 0.94, "PbPb 3 nb^{-1}")

pad2 = c.cd(2)
pad2.SetLeftMargin(0.13)
pad2.SetRightMargin(0.04)
pad2.SetTopMargin(0.08)
pad2.SetBottomMargin(0.15)
h1SnB.SetStats(0)
h1SnB.SetTitle(";M_{#piKp} (GeV);Events / 2 MeV")
h1SnB.SetLineColor(39)
h1SnB.SetMarkerStyle(20)
h1SnB.SetMarkerSize(1.2)
h1SnB.GetXaxis().SetTitleSize(0.05)
h1SnB.GetYaxis().SetTitleSize(0.05)
h1SnB.GetXaxis().SetRangeUser(r.ana.fitRangeLw, r.ana.fitRangeUp)
h1SnB.GetXaxis().SetTitleOffset(1.1)
h1SnB.GetYaxis().SetTitleOffset(1)
h1SnB.GetXaxis().CenterTitle()
h1SnB.GetYaxis().CenterTitle()
h1SnB.SetMaximum(h1SnB.GetMaximum() * 1.85)
h1SnB.Draw("EP")
line.Draw()
tex.DrawLatexNDC(0.35, 0.8, "%.1f < p_{T} <%.1fGeV" % (pTMin, pTMax))
tex.DrawLatexNDC(0.35, 0.72, "|y| < 1")
texCMS.DrawLatexNDC(0.13, 0.94, "CMS Phase II #it{Simulation}")
texMTD.DrawLatexNDC(0.78, 0.76, "no MTD")
texAA.DrawLatexNDC(0.96, 0.94, "PbPb 3 nb^{-1}")

c.Print("plots/ProjectedSignalMass_pT%.1f_%.1f_y%.1f_%.1f_toD0Ratio%s.png" % (pTMin, pTMax, yMin, yMax, sys.argv[1]))
c.Print("plots/ProjectedSignalMass_pT%.1f_%.1f_y%.1f_%.1f_toD0Ratio%s.pdf" % (pTMin, pTMax, yMin, yMax, sys.argv[1]))
c.Print("plots/ProjectedSignalMass_pT%.1f_%.1f_y%.1f_%.1f_toD0Ratio%s.C" % (pTMin, pTMax, yMin, yMax, sys.argv[1].replace('.', 'p')))
