#!/usr/bin/bash
if [[ ! -d HYDJETS/log ]]; then
  echo "Create HYDJETS and HYDJETS/log"
  mkdir -p HYDJETS/log
else
  echo "HYDJETS and HYDJETS/log exist"
fi
./setup_tarball.sh
echo "tarball.tar.gz is ready"
cp tarball.tar.gz HYDJETS
echo "Copied tarball.tar.gz to HYDJETS"
