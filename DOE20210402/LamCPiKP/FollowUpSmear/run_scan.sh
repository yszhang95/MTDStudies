#!/usr/bin/bash
source /cvmfs/sft.cern.ch/lcg/views/LCG_99/x86_64-centos7-gcc10-opt/setup.sh
tar zxf tarball.tar.gz
cp lists/$1 $1
root -b <<- _EOF_
.L fillHyJetsMass_Scan.C+
fillHyJetsMass("$1", $2, $3)
.q
_EOF_
