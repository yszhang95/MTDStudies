# README
## D0 reproduction for DOE review
Do the following:
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_4_0_mtd5
cd CMSSW_10_4_0_mtd5/src
git clone https://github.com/stahlleiton/VertexCompositeAnalysis.git -b ParticleFitter_10_4_X_mtd
scram b
```
```
HyJets_PhaseIIMTD_D0_mc_ntuple.py
PromptD0_PhaseIIMTD_mc_ntuple.py
crabSkimAndTree_PhaseII_HYDJETS_D0_Ntuple.py
crabSkimAndTree_PhaseII_PromotD0_Ntuple.py
```
Use the scripts above and copy
`VertexCompositeSelector.cc` to
`VertexCompositeAnalysis/VertexCompositeAnalyzer/plugins/VertexCompositeSelector.cc`.
## Tree production
```
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_10_4_0_mtd5
cd CMSSW_10_4_0_mtd5/src
git cms-merge-topic stahlleiton:RERECO_MTD_CMSSW_10_4_0_mtd5_V2
cmsenv
git clone https://github.com/yszhang95/VertexCompositeAnalysis.git -b 10_4_X_mtd_TEMP
scram b
cmsenv
voms-proxy-init --voms cms
source /cvmfs/cms.cern.ch/common/crab-setup.sh
cd VertexCompositeAnalysis/VertexCompositeProducer/test
```
### Ds production for DOE review
Copy the files under `configs/Ds_PhiPi` to proper directory.
### B production for DOE review
Copy the files under `configs/B_DPi` to proper directory.

