#!/usr/bin/env python
import ROOT as r
r.gROOT.SetBatch()

yRanges = ['0to1', '1to2', '2to3']

mtd = ['', '_noMTD']

var = ['Sig', 'S', 'B']

fs = ['sig_MB.root', 'sig_0_10.root', 'sig_30_50.root']

cents = ['MB', '0_10', '30_50']

for f,cent in zip(fs, cents):
  rf = r.TFile(f)
  for d in mtd:
    c = r.TCanvas('c_%s%s' % (cent, d), '', 1500, 500)
    c.Divide(3, 1)
    for v in var:
      print v
      i = 1
      for y in yRanges:
        c.cd(i)
        i = i+1
        g = rf.Get('g%sVsPt_%s%s' % (v, y, d))
        g.SetTitle('%sVsPt y%s%s centrality %s;pT (GeV);%s' % (v, y, d, cent, v))
        g.Draw("AP")
      c.Print('plots/cent%s_%sVsPt%s.pdf' % (cent, v, d))
      c.Print('plots/cent%s_%sVsPt%s.png' % (cent, v, d))
