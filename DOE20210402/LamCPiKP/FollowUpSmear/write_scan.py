#!/usr/bin/env python

nRMSBTL = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5]
nRMSETL = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5]

# ls -l ../lists
from os import listdir
from os.path import isfile, join
mylists = [l for l in listdir("./lists") if isfile(join("./lists", l)) and not ('woPID' in l)]
#print (mylists)

with open("scanRMS.jdl", "w") as f:
  script = '''
Universe   = vanilla
Executable = run_scan.sh
Log        = log/$(Process).log
Output     = log/$(Process).out
Error      = log/$(Process).err
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
initialdir = ./batchScan
transfer_input_files = tarball.tar.gz
+JobFlavour = "microcentury"
'''
  count = 0
  for etlCut in nRMSETL:
    for btlCut in nRMSBTL:
      for l in mylists:
        script += "# %d\n" % count
        oname = "HyJetsMassHists_reRECO_%s_TDR_%sBTL_%sETLRMS.root" % (l, str(btlCut).replace('.', 'p'), str(etlCut).replace('.', 'p'))
        script += "Arguments = %s %.1f %.1f\n" % (l, btlCut, etlCut)
        script += "transfer_output_files = %s\n" % oname
        script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
        script += "Queue\n\n"
        count = count+1
  f.write(script)
