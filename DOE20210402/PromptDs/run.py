#!/usr/bin/env python
import ROOT as r
r.gROOT.LoadMacro('helpers.cc+')
Phi = r.Particle(333)
Phi.selfConj(True);
Phi.addDaughter(r.Particle(-321))
Phi.addDaughter(r.Particle(+321))
Ds = r.Particle(431)
Ds.addDaughter(r.Particle(+211))
Ds.addDaughter(Phi)
print(Ds.daughter(0).id())
print(Ds.daughter(1).id())
Ds.flipFlavor()
print(Ds.daughter(1).id())
print(Ds.daughter(0).id())

ts = r.TStopwatch()
ts.Start()
r.genMatchDsMass('Ds.list', 'ds_ana_mc', Ds, -1)
r.genMatchFSDsMass('Ds.list', 'ds_ana_mc', Ds, -1)
ts.Stop()
ts.Print()
