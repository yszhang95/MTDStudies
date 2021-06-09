#!/usr/bin/env python

# nRMS = {1., 1.1, 1.2, 1.3, 1.4, 1.5}
# 2 layer 1 layer 0 layer
# 35 ps, 45 ps, 50 ps, 65 ps

#nRMS = [1.8, 2.0, 2.3, 2.5]
nRMS = [1.0, 1.3, 1.5]

jobs = {
    "2layer" : {"res" : 0.045, "inEff" : "false", "disableETL" : "false"},
    "1layer_50" : {"res" : 0.050, "inEff" : "true", "disableETL" : "false"},
    "1layer_65" : {"res" : 0.065, "inEff" : "true", "disableETL" : "false"},
    "0layer" : {"res" : 0.065, "inEff" : "true", "disableETL" : "true"}
    }

with open("run_signal_200evts.sh", "w") as f:
  script = '''#!/usr/bin/bash\n'''
  count = 0
  for k, v in jobs.items():
    if k == "0layer":
      script += "root -b <<- _EOF_\n"
      script += ".L fillLamC3PMass_200evts.C+\n"
      script += "fillLamC3PMass(%s, %.3ff, %.1ff, %s)\n" % (v["inEff"], v["res"], 1.0, v["disableETL"])
      script += ".q\n"
      script += "_EOF_\n"
    else:
      for rms in nRMS:
        script += "root -b <<- _EOF_\n"
        script += ".L fillLamC3PMass_200evts.C+\n"
        script += "fillLamC3PMass(%s, %.3ff, %.1ff, %s)\n" % (v["inEff"], v["res"], rms, v["disableETL"])
        script += ".q\n"
        script += "_EOF_\n"
  f.write(script)
