from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException
from CRABClient.UserUtilities import config

config = config()
config.section_('General')
config.General.transferLogs = False
config.General.transferOutputs = True
config.General.workArea = 'crab_projects'
config.General.requestName = 'HydJets_mc_mtd_D0_NTUPLE_20210316v2'
config.section_('JobType')
config.JobType.psetName = 'HyJets_PhaseIIMTD_D0_mc_ntuple.py'
config.JobType.pluginName = 'Analysis'
config.JobType.maxJobRuntimeMin = 350
config.JobType.numCores = 8
config.JobType.maxMemoryMB = 15000
config.section_('Data')
config.Data.inputDataset = '/MinBias_Hydjet_Drume5_5p5TeV_TuneCP5_Pythia8/anstahll-HydJets_mc_mtd_D0_ANASKIM_20190308-70eaed48d1aedce7130b6d7abc8d272f/USER'
config.Data.outputDatasetTag = config.General.requestName
config.Data.publication = False
config.Data.unitsPerJob = 1
config.Data.ignoreLocality = True
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.outLFNDirBase = '/store/group/phys_heavyions/MTD/yousen/VertexCompositeAnalysis/%s' % (config.General.requestName)
config.section_('Site')
config.Site.whitelist = ['T1_FR_*', 'T1_US_*', 'T2_CH_*', 'T2_US_*', 'T2_FR_*']
config.Site.storageSite = 'T2_CH_CERN'
config.JobType.allowUndistributedCMSSW = True
