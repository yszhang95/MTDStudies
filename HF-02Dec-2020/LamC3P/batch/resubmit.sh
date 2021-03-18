#!/usr/bin/bash
failedjobs=$(grep held log/*log | cut -d : -f 1)
jobnums=()
for j in ${failedjobs[@]}; do
  b=$(basename -- $j)
  jobnums+=("${b%.*}")
done
#echo ${jobnums[@]}
sed -n "/Universe/,/+Job/p" submit_condor.jdl > resubmit.jdl
for j in ${jobnums[@]}; do
  #perl -ne "print if /${j}/" submit_condor.jdl
  sed -n "/# ${j}/,/Queue/p" submit_condor.jdl >> resubmit.jdl
done
