#!/usr/bin/env python3
import subprocess
for nrms in range(0, 6):
  cmd = ['hadd', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_45ps_2Layer_1p%dRMS.root' % nrms]
  for i in range(1, 7):
    cmd.append("HydjetsMassHists/HyJetsMassHists_reRECO_hyjets_sample_wPID_nevts4950_part%d.list_45ps_2Layer_1p%dRMS.root" % (i, nrms))
  subprocess.run(cmd)

  cmd = ['hadd', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_65ps_1Layer_1p%dRMS.root' % nrms]
  for i in range(1, 7):
    cmd.append("HydjetsMassHists/HyJetsMassHists_reRECO_hyjets_sample_wPID_nevts4950_part%d.list_65ps_1Layer_1p%dRMS.root" % (i, nrms))
  subprocess.run(cmd)

  cmd = ['hadd', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_50ps_1Layer_1p%dRMS.root' % nrms]
  for i in range(1, 7):
    cmd.append("HydjetsMassHists/HyJetsMassHists_reRECO_hyjets_sample_wPID_nevts4950_part%d.list_50ps_1Layer_1p%dRMS.root" % (i, nrms))
  subprocess.run(cmd)

cmd = ['hadd', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_NoETL.root']
for i in range(1, 7):
  cmd.append("HydjetsMassHists/HyJetsMassHists_reRECO_hyjets_sample_wPID_nevts4950_part%d.list_NoETL.root" % (i))
subprocess.run(cmd)
