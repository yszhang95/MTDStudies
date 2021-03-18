#!/usr/bin/env python3

inEff = {"true" : "True", "false" : "False"}
whereLost = {"innerLost" : 0, "outerLost" : 1}
macros = {"fillHyJetsMass_0p5Layer.C",
  "fillHyJetsMass_1Layer.C", "fillHyJetsMass_1p5Layer.C"}

# ls -l ../lists
from os import listdir
from os.path import isfile, join
mylists = [l for l in listdir("../lists") if isfile(join("../lists", l))]
#print (mylists)

with open("submit_condor.jdl", "w") as f:
  script = '''
Universe   = vanilla
Executable = run.sh
Log        = log/$(Process).log
Output     = log/$(Process).out
Error      = log/$(Process).err
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
initialdir = .
transfer_input_files = tarball.tar.gz
+JobFlavour = "microcentury"

'''

  count = 0
  for m in macros:
    for l in mylists:
      for (keff, veff) in inEff.items():
        if '1Layer' in m:
          script += "# %d\n" % count
          oname = "HyJetsMassHists_reRECO_%s_1layer_inEff%s.root" % (l, veff)
          script += "Arguments = %s %s None %s\n" % (m, l, keff)
          script += "transfer_output_files = %s\n" % oname
          script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
          script += "Queue\n\n"
          count = count+1
        else:
          for (kwl, vwl) in whereLost.items():
            script += "# %d\n" % count
            script += "Arguments = %s %s %s %s\n" % (m, l, kwl, keff)
            oname = ""
            if '1p5' in m:
              oname += "HyJetsMassHists_reRECO_%s_1p5layer_%d_inEff%s.root" % (l, vwl, veff)
            else:
              if '0p5' in m:
                oname += "HyJetsMassHists_reRECO_%s_0p5layer_%d_inEff%s.root" % (l, vwl, veff)
            script += "transfer_output_files = %s\n" % oname
            script += "transfer_output_remaps = \"%s=out/%s\"\n" % (oname, oname)
            script += "Queue\n\n"
            count = count+1
#
  f.write(script)
