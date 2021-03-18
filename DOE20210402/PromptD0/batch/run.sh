#!/usr/bin/bash
source /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.12.06/x86_64-centos7-gcc48-opt/root/bin/thisroot.sh
tar zxf tarball.tar.gz
root -b "fillHyJetsMass.C(\"$1\")"

rm -r *.h *.C *.list .rootlogon.C lists
