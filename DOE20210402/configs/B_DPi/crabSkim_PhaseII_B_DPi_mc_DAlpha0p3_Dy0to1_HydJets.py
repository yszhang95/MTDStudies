import CRABClient
from WMCore.Configuration import Configuration
config = Configuration()

from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PbPbSkim_PhaseIIMTD_B_DPi_mc_DAlpha0p3_Dy0to1_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxJobRuntimeMin = 200
config.JobType.maxMemoryMB = 5000

config.section_("Data")
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1
config.Data.publication = True
#config.Data.ignoreLocality = True
config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T2_US_MIT'
#config.Site.whitelist = ['T1_US_*','T2_US_*','T1_FR_*','T2_FR_*','T2_CH_CERN']

def submit(config):
    try:
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

dataMap = {
    "HydJets" : {"PD" : '/MinBias_Hydjet_Drume5_5p5TeV_TuneCP5_Pythia8/PhaseIIMTDTDRAutumn18DR-NoPU_103X_upgrade2023_realistic_v2-v2/FEVT'},
    }

for key, val in dataMap.items():
    config.General.requestName = 'PbPb5p5TeV_'+key+'_B_DPi_DAlpha0p3_Dy0to1_20210320'
    config.Data.inputDataset = val["PD"]
    config.Data.outputDatasetTag = config.General.requestName
    config.Data.outLFNDirBase = '/store/user/yousen/MTD/B_DPi/HYDJET_NoPU/'
    print("Submitting CRAB job for: "+val["PD"])
    submit(config)
