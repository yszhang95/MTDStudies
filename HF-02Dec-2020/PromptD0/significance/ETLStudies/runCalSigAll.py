#!/usr/bin/env python
import ROOT as r
r.gROOT.SetBatch(1)
WhereLost = [0, 1]
Layers = ["0p5", "1", "1p5", "0", "2"]
InEff = ["True", "False"]
r.gROOT.LoadMacro("calSigAll.C")
for l in Layers:
  if l is "0" or l is "2":
    sF = "PromptDMassHists_reRECO_all_" + l + "layer.root"
    bF = "HyJetsMassHists_reRECO_tdr.list_" + l + "layer.root"
    r.calSigAll(sF, bF)
  else:
    for w in WhereLost:
      for e in InEff:
        if not l is "1":
          sF = "PromptDMassHists_reRECO_all_" + l + "layer_" + str(w) + "_inEff" + e + ".root"
          bF = "HyJetsMassHists_reRECO_tdr.list_" + l + "layer_" + str(w) + "_inEff" + e + ".root"
        else:
          sF = "PromptDMassHists_reRECO_all_" + l + "layer_inEff" + e + ".root"
          bF = "HyJetsMassHists_reRECO_tdr.list_" + l + "layer_inEff" + e + ".root"
        r.calSigAll(sF, bF)
