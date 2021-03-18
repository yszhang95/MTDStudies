#!/usr/bin/bash
echo "Packing tarball.tar.gz"
tar zcf tarball.tar.gz ../lists/* ../*.h ../*.C ../.rootlogon.C ../*list
