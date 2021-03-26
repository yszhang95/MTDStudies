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
config.JobType.psetName = 'PbPbSkimAndTreePhaseIIMTD_Ds_PhiPi_mc_cfg.py'
config.JobType.allowUndistributedCMSSW = True
config.JobType.maxJobRuntimeMin = 80
config.JobType.maxMemoryMB = 2500

config.section_("Data")
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
config.Site.whitelist = ['T1_US_*','T2_US_*','T1_FR_*','T2_FR_*','T2_CH_CERN']

def submit(config):
    try:
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

dataMap = {
    "DsNoPU" : {"PD" : '/Ds_PhiPi_prompt_pt0_y4_5p5TeV_TuneCP5_Pythia8/PhaseIIMTDTDRAutumn18DR-NoPU_103X_upgrade2023_realistic_v2-v1/FEVT'},
    }

for key, val in dataMap.items():
    config.General.requestName = 'PbPb5p5TeV_'+key+'_Ds_PhiPi_DsSAMPLE_20210321'
    config.Data.inputDataset = val["PD"]
    config.Data.outputDatasetTag = config.General.requestName
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/MTD/yousen/Ds_PhiPi/DsSAMPLE_NoPU/'
    print("Submitting CRAB job for: "+val["PD"])
    submit(config)
