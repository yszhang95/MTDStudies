#!/usr/bin/env python

nRMS = [1.0, 1.3, 1.5]
#nRMS = [2.0, 2.3, 2.5, 3.0]

jobs = {
    "2layer" : {"res" : 0.045, "inEff" : "false", "disableETL" : "false", "ofile" : "HyJetsMassHists_reRECO_%s_45ps_2Layer_%sRMS.root"},
    "1layer_50" : {"res" : 0.050, "inEff" : "true", "disableETL" : "false", "ofile" : "HyJetsMassHists_reRECO_%s_50ps_1Layer_%sRMS.root"},
    "1layer_65" : {"res" : 0.065, "inEff" : "true", "disableETL" : "false", "ofile" : "HyJetsMassHists_reRECO_%s_65ps_1Layer_%sRMS.root"},
    }

# ls -l ../lists
from os import listdir
from os.path import isfile, join
mylists = [l for l in listdir("./lists") if isfile(join("./lists", l)) and ('woPID' in l)]
#print (mylists)

with open("submit_200evts.jdl", "w") as f:
  script = '''
Universe   = vanilla
Executable = run_200evts.sh
Log        = log/$(Process).log
Output     = log/$(Process).out
Error      = log/$(Process).err
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
initialdir = ./batch200evts
transfer_input_files = tarball.tar.gz
+JobFlavour = "microcentury"
'''
  count = 0
  for k, v in jobs.items():
    if k == "0layer":
      for l in mylists:
        script += "# %d\n" % count
        oname = ""
        if "NoETL" in v["ofile"]:
          oname = v["ofile"] % (l)
        else:
          oname = v["ofile"] % (l, str(rms).replace('.', 'p'))
        script += "Arguments = %s %s %.3f %.1f %s true\n" % (l, v["inEff"], v["res"], rms, v["disableETL"])
        script += "transfer_output_files = %s\n" % oname
        script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
        script += "Queue\n\n"
        count = count+1
    else:
      for rms in nRMS:
        for l in mylists:
          script += "# %d\n" % count
          oname = ""
          if "NoETL" in v["ofile"]:
            oname = v["ofile"] % (l)
          else:
            oname = v["ofile"] % (l, str(rms).replace('.', 'p'))
          script += "Arguments = %s %s %.3f %.1f %s true\n" % (l, v["inEff"], v["res"], rms, v["disableETL"])
          script += "transfer_output_files = %s\n" % oname
          script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
          script += "Queue\n\n"
          count = count+1
  f.write(script)
