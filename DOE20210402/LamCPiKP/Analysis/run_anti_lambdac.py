#/usr/bin/env python
import ROOT as r
r.gSystem.Load('${OPENHF2020TOP}/Utilities/lib/libMyTreeReader.so')
r.gROOT.LoadMacro('./analysis.cc+')

LambdaC = r.Particle(4122)
LambdaC.addDaughter(r.Particle(+211))
LambdaC.addDaughter(r.Particle(-321))
LambdaC.addDaughter(r.Particle(+2212))

topoCut = r.ana.TopoCut(0., 99999., 999999, 0.0)
kinsCut = r.ana.KineCut(0., 1000., 0., 3.)

ts = r.TStopwatch()
ts.Start()
#r.analysis('LcPlusWPID.list', 'lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
#r.analysis('LcPlusWPID.list', 'anti_lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
#r.analysisTrack('LcPlusWPID.list', 'lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
#r.analysisTrack('LcPlusWPID.list', 'anti_lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
#r.analysis('test.list', 'lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
r.analysis('A.list', 'anti_lamc_ana_mc', LambdaC, topoCut, kinsCut, -1, True, True, True)
#r.analysis('A.list', 'lamc_ana_mc', LambdaC, topoCut, kinsCut, 50000, True, True, True)
ts.Stop()
ts.Print()
