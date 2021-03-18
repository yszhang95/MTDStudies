#!/usr/bin/env python
import ROOT as r
r.gROOT.SetBatch(1)
WhereLost = [0, 1]
Layers = ["0p5", "1", "1p5", "0"]
InEff = ["True", "False"]
r.gROOT.LoadMacro("calSigAll.C")
for w in WhereLost:
  for l in Layers:
    if l is "0":
      sF = "../LamC3P/LamC3PMassHists_reRECO_" + l + "layer.root"
      bF = "../HYDJETS/HyJetsMassHists_reRECO_sample_wPID_nevts4950_part_" + l + "layer.root"
      r.calSigAll(sF, bF)
    else:
      for e in InEff:
        if not l is "1":
          sF = "../LamC3P/LamC3PMassHists_reRECO_" + l + "layer_" + str(w) + "_inEff" + e + ".root"
          bF = "../HYDJETS/HyJetsMassHists_reRECO_sample_wPID_nevts4950_part_" + l + "layer_" + str(w) + "_inEff" + e + ".root"
        else:
          sF = "../LamC3P/LamC3PMassHists_reRECO_" + l + "layer_inEff" + e + ".root"
          bF = "../HYDJETS/HyJetsMassHists_reRECO_sample_wPID_nevts4950_part_" + l + "layer_inEff" + e + ".root"
        print sF
        print bF
        #if l is "1" and e is "False":
          #continue
        r.calSigAll(sF, bF)
