#!/usr/bin/env python
import copy
import ROOT as r
r.gROOT.LoadMacro('genMatch.cc+')
Dbar = r.Particle(-421)
Dbar.addDaughter(r.Particle(-211))
Dbar.addDaughter(r.Particle(321))
Bplus = r.Particle(521)
Bplus.addDaughter(r.Particle(211))
Bplus.addDaughter(Dbar)

D = r.Particle(421)
D.addDaughter(r.Particle(211))
D.addDaughter(r.Particle(-321))
Bminus = r.Particle(-521)
Bminus.addDaughter(r.Particle(-211))
Bminus.addDaughter(D)

#r.genMatchBMass('b.list', 'bcharged_ana', Bplus, -1)
#r.genMatchBMass('b.list', 'bcharged_ana', Bplus, 249)
r.genMatchFSBMass('b.list', 'bcharged_ana', Bplus, -1)
#r.genMatchFSBMass('bkg.list', 'bcharged_ana', Bplus, -1)
