#!/usr/env/bin python
import sys
sys.path.append("../../Utilities/Ana/")

import ROOT as r
import calculator
import histManip
r.gROOT.SetBatch()

def fitSides(hB):
  #r.ana.reject = True
  r.ana.reject = False
  #f = r.TF1(hB.GetName()+"_sidefit", r.ana.f2ndpoly, r.ana.fitRangeLw, r.ana.fitRangeUp, 4)
  f = r.TF1(hB.GetName()+"_sidefit", "ROOT::Math::Chebyshev2(x, [0], [1], [2])", r.ana.fitRangeLw, r.ana.fitRangeUp)
  #f = r.TF1(hB.GetName()+"_sidefit", "[0]*cos([1]*x+[2])", r.ana.fitRangeLw, r.ana.fitRangeUp)
  #f.FixParameter(3, 1.)
  val1 = hB.GetBinContent(hB.GetXaxis().FindBin(r.ana.fitRangeLw))
  val2 = hB.GetBinContent(hB.GetXaxis().FindBin(r.ana.fitRangeUp))
  par1 = (val2-val1)/(r.ana.fitRangeUp-r.ana.fitRangeLw)
  par0 = val1 - par1*r.ana.fitRangeLw
  f.SetParameter(0, 4.53825e+10)
  f.SetParameter(1, -7.93580e+09)
  f.SetParameter(2, 10)
  #f.SetParLimits(2, -1000, 0)
  hB.Fit(f, 'qR', '', r.ana.fitRangeLw, r.ana.fitRangeUp)
  hB.Fit(f, 'qR', '', r.ana.fitRangeLw, r.ana.fitRangeUp)
  hB.Fit(f, 'R', '', r.ana.fitRangeLw, r.ana.fitRangeUp)
  f.ReleaseParameter(3)
  c = r.TCanvas("c", "", 500, 400)
  hB.Draw()
  c.Print("plots/%s.pdf" % f.GetName())
  c.Print("plots/%s.png" % f.GetName())
  return f

useMTD = True
calculator.setup()
h3scale = calculator.scaleHist(5.6, 'CMS-D0.root', 'Table 3/Hist1D_y1', 'output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root', 'hGenYVsPt', -1, 1.)

h3signal = {}
h3background = {}


hSignalMass = {}
hBkgMass = {}

h3signal['unscale'] = calculator.getSignal('output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD, None, False)
(h3background['unscale'], scale) = calculator.getBackground('output/HYDJETS_bcharged_ana_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD, False)

hSignalMass['unscale'] = histManip.getMassDistri(h3signal['unscale'], 0, 1, 3, 4, 's_unscal')
hBkgMass['unscale'] = histManip.getMassDistri(h3background['unscale'], 0, 1, 3, 4, 'b_unscal')

h3signal['scale'] = calculator.getSignal('output/Ds_ds_ana_mc_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD, h3scale)
(h3background['scale'], scale) = calculator.getBackground('output/HYDJETS_bcharged_ana_AllEntries_Ds_PhiPi_BottomToTop.root', useMTD)

hSignalMass['scale'] = histManip.getMassDistri(h3signal['scale'], 0, 1, 3, 4, 's_scal')
hBkgMass['scale'] = histManip.getMassDistri(h3background['scale'], 0, 1, 3, 4, 'b_scal')

#pars = [500, 1.96827, 0.005, 500, 1.96827, 0.01]
pars = [500, 1.96827, 0.005]
signalShape = histManip.fitRawSignal(hSignalMass['unscale'], *pars)

nSignal = h3signal['scale'].Integral()
hSmoothSignal = r.TH1D(hSignalMass['unscale'].GetName()+'smooth', ";M (GEV);", r.ana.nmass, r.ana.massMin, r.ana.massMax)
histManip.generateSmoothSignal(hSmoothSignal, signalShape, nSignal)

bkgScale = hBkgMass['scale'].Integral()/hBkgMass['unscale'].Integral()
hSmoothBkg = histManip.generateSmoothBackground(hBkgMass['unscale'], bkgScale)
hSmoothBkgUnscale = histManip.generateSmoothBackground(hBkgMass['unscale'], None)

#f = fitSides(hBkgMass['unscale'])
f = fitSides(hSmoothBkg)
f.SetRange(1.8, 2.1)
hSub = r.TH1D('hsub', '', r.ana.nmass, r.ana.massMin, r.ana.massMax)
n = hSmoothBkg.Integral(hSmoothBkg.FindBin(r.ana.fitRangeLw), hSmoothBkg.FindBin(r.ana.fitRangeUp))
#hSub.FillRandom(f.GetName(), long(n))
r.ana.reject = False
#f.SetParameter(3, norm)

hAll = histManip.generateSumHist(hSmoothSignal, hSmoothBkg, 3, 4, 0, 1)
hAll.Sumw2()
hAll.Add(f, -1)

c = r.TCanvas('c', "", 500, 400)
hAll.SetLineColor(r.kBlue)
hAll.GetListOfFunctions().Clear()
hAll.Draw("E")
#hAll.GetYaxis().SetRangeUser(-1e3, hSmoothSignal.GetMaximum()*1.1)
hAll.GetXaxis().SetRangeUser(r.ana.fitRangeLw, r.ana.fitRangeUp)
c.Print('plots/%s_sub.pdf' % hAll.GetName())
c.Print('plots/%s_sub.png' % hAll.GetName())
