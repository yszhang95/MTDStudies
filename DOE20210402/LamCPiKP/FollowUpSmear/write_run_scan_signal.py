#!/usr/bin/env python

nRMSBTL = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5]
nRMSETL = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5]

with open("run_scan_signal.sh", "w") as f:
  script = '''#!/usr/bin/bash\n'''
  count = 0
  for etlCut in nRMSETL:
    for btlCut in nRMSBTL:
      script += "root -b <<- _EOF_\n"
      script += ".L fillLamC3PMass_Scan.C+\n"
      script += "fillLamC3PMass(%.1ff, %.1ff)\n" % (btlCut, etlCut)
      script += ".q\n"
      script += "_EOF_\n"
  f.write(script)
