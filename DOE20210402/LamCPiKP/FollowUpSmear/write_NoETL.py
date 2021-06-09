#!/usr/bin/env python

from os import listdir
from os.path import isfile, join
mylists = [l for l in listdir("./lists") if isfile(join("./lists", l)) and ('woPID' in l)]
#print (mylists)

with open("sub_NoETL.jdl", "w") as f:
  script = '''
Universe   = vanilla
Executable = runNoETL.sh
Log        = log/$(Process).log
Output     = log/$(Process).out
Error      = log/$(Process).err
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
initialdir = ./batchNoETL
transfer_input_files = tarball.tar.gz
+JobFlavour = "microcentury"
'''
  count = 0
  for l in mylists:
    script += "# %d\n" % count
    oname = "HyJetsMassHists_reRECO_%s_NoETL.root" % l
    script += "Arguments = %s\n" % l
    script += "transfer_output_files = %s\n" % oname
    script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
    script += "Queue\n\n"
    count = count+1
  f.write(script)
