#!/usr/bin/env python3
with open('submit.jdl', 'w') as f:
  cmds = '''
# this is config for submitting condor jobs
Universe   = vanilla
Executable = run.sh
Log        = log/$(Process).log
Output     = log/$(Process).out
Error      = log/$(Process).err
initialdir = HYDJETS
transfer_input_files = tarball.tar.gz
requirements = (OpSysAndVer =?= "CentOS7")
+JobFlavour   = "microcentury"

should_transfer_files = YES
when_to_transfer_output = ON_EXIT
  '''
  for i in range(0, 28):
    cmds +='''
Arguments  = lists/HYDJETS_ALL.list.%02d
queue
  ''' % i
  f.write(cmds)
