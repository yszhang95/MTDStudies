#!/usr/bin/env python
import ROOT as r
r.gROOT.LoadMacro('helpers2.cc+')
Phi = r.Particle(333)
Phi.selfConj(True);
Phi.addDaughter(r.Particle(-321))
Phi.addDaughter(r.Particle(+321))
Ds = r.Particle(431)
Ds.addDaughter(r.Particle(+211))
Ds.addDaughter(Phi)

#topoCut = r.ana.TopoCut(0.0, 99999., 0.6, 0.0) # old
topoCut = r.ana.TopoCut(0.0, 99999., 10000, 0.0) # old
#topoCut = r.ana.TopoCut(0.0, 99999., 0.45, 0.0)
kinsCut = r.ana.KineCut(0., 1000., 0., 3.)

ts = r.TStopwatch()
ts.Start()
r.genMatchFSDsMass('Dsnew.list', 'ds_ana_mc', Ds, topoCut, kinsCut, -1, True, True, True)
#r.genMatchFSDsMass('DsHydJets_pT2p95_y0to1p05.list', 'ds_ana_mc', Ds, topoCut, kinsCut, -1, False, False, False)
#r.genMatchFSDsMass('DsHydJets_pT2p95_y0p95to2p05.list', 'ds_ana_mc', Ds, topoCut, kinsCut, -1, False, False, False)
#r.genMatchFSDsMass('DsHydJets_pT2p95_y1p95to3p05.list', 'ds_ana_mc', Ds, topoCut, kinsCut, -1, False, False, False)
#r.genMatchFSDsMass('Ds.list.old', 'ds_ana_mc', Ds, topoCut, kinsCut, -1, True, True, True) # old
#r.genMatchFSDsMass('HYDJETS.list', 'bcharged_ana', Ds, topoCut, kinsCut, -1, False, False, False) # old
ts.Stop()
ts.Print()
