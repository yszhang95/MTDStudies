#!/usr/bin/env python3
import subprocess

#NRMS = ['2p0', '2p3', '2p5', '3p0']
NRMS = ['1p0', '1p3', '1p5']
for nrms in NRMS:
  subprocess.run(['rm', '200evts/HyJetsMassHists_reRECO_woPID_45ps_2Layer_%sRMS.root' % nrms])
  cmd = ['hadd', '200evts/HyJetsMassHists_reRECO_woPID_45ps_2Layer_%sRMS.root' % nrms]
  for i in range(1, 6):
    cmd.append("batch200evts/out/HyJetsMassHists_reRECO_hyjets_woPID%d.list_45ps_2Layer_%sRMS.root" % (i, nrms))
  subprocess.run(cmd)

  subprocess.run(['rm', '200evts/HyJetsMassHists_reRECO_woPID_65ps_1Layer_%sRMS.root' % nrms])
  cmd = ['hadd', '200evts/HyJetsMassHists_reRECO_woPID_65ps_1Layer_%sRMS.root' % nrms]
  for i in range(1, 6):
    cmd.append("batch200evts/out/HyJetsMassHists_reRECO_hyjets_woPID%d.list_65ps_1Layer_%sRMS.root" % (i, nrms))
  subprocess.run(cmd)

  subprocess.run(['rm', '200evets/HyJetsMassHists_reRECO_woPID_50ps_1Layer_%sRMS.root' % nrms])
  cmd = ['hadd', '200evts/HyJetsMassHists_reRECO_woPID_50ps_1Layer_%sRMS.root' % nrms]
  for i in range(1, 6):
    cmd.append("batch200evts/out/HyJetsMassHists_reRECO_hyjets_woPID%d.list_50ps_1Layer_%sRMS.root" % (i, nrms))
  subprocess.run(cmd)
