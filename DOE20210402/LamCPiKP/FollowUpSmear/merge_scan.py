#!/usr/bin/env python3
import subprocess
for btlCut in range(0, 6):
  for etlCut in range(0, 6):
    subprocess.run(['rm', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_TDR_1p%dBTL_1p%dETLRMS.root' % (btlCut, etlCut)])
    cmd = ['hadd', 'HydjetsMassHists/HyJetsMassHists_reRECO_wPID_TDR_1p%dBTL_1p%dETLRMS.root' % (btlCut, etlCut)]
    for i in range(1, 7):
      cmd.append("HydjetsMassHists/HyJetsMassHists_reRECO_hyjets_sample_wPID_nevts4950_part%d.list_TDR_1p%dBTL_1p%dETLRMS.root" % (i, btlCut, etlCut))
    subprocess.run(cmd)
    #print (cmd)

