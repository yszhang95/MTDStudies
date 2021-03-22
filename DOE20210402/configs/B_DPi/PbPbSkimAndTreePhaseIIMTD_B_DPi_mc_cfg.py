import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process('ANASKIM',eras.Phase2C4_timing_layer_bar)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Limit the output messages
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 200
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# Define the input source
process.source = cms.Source("PoolSource",
   fileNames = cms.untracked.vstring('/store/mc/PhaseIIMTDTDRAutumn18DR/B_DPi_pt0_y4_5p5TeV_TuneCP5_Pythia8/FEVT/NoPU_103X_upgrade2023_realistic_v2-v1/80000/B79911C0-FA0C-134A-9B79-B7DF250E059A.root'),
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

# Set the global tag
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_upgrade2023_realistic_v2', '')

# MTD RE-RECO
process.reconstruction_step = cms.Path()
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.reconstruction_step += cms.Sequence(process.mtdClusters * process.mtdTrackingRecHits)

# PV RE-RECO
fixedT0Error = cms.double(0.035) #put a constant 0.035 [ns] error for each track
process.trackExtenderWithMTDnoPID = process.trackExtenderWithMTD.clone(UseVertex = cms.bool(False),
                                                                       fixedT0Error = fixedT0Error)
process.offlinePrimaryVertices4DnoPID = process.unsortedOfflinePrimaryVertices4D.clone(TrackTimesLabel = cms.InputTag("trackExtenderWithMTDnoPID:generalTrackt0"),
                                                                                       TrackTimeResosLabel = cms.InputTag("trackExtenderWithMTDnoPID:generalTracksigmat0"))
process.offlinePrimaryVertices4DnoPID.TkClusParameters.TkDAClusParameters.tmerge = cms.double(1.0)
process.offlinePrimaryVertices4DnoPID.TkClusParameters.TkDAClusParameters.zmerge = cms.double(1.0)
process.offlinePrimaryVertices4DnoPID.TkFilterParameters.minPixelLayersWithHits = cms.int32(3)
process.offlinePrimaryVertices4DnoPID.TkFilterParameters.maxD0Significance = cms.double(2.0)
process.offlinePrimaryVertices4DnoPID.TkFilterParameters.minPt = cms.double(0.7)
process.offlinePrimaryVertices4DnoPID.TkFilterParameters.trackQuality = cms.string("highPurity")
process.tofPIDnoPID = process.tofPID.clone(vtxsSrc = cms.InputTag('offlinePrimaryVertices4DnoPID'),
                                           t0Src = cms.InputTag("trackExtenderWithMTDnoPID:generalTrackt0"),
                                           tmtdSrc = cms.InputTag("trackExtenderWithMTDnoPID:generalTracktmtd"),
                                           sigmat0Src = cms.InputTag("trackExtenderWithMTDnoPID:generalTracksigmat0"),
                                           sigmatmtdSrc = cms.InputTag("trackExtenderWithMTDnoPID:generalTracksigmatmtd"),
                                           pathLengthSrc = cms.InputTag("trackExtenderWithMTDnoPID:generalTrackPathLength"),
                                           pSrc = cms.InputTag("trackExtenderWithMTDnoPID:generalTrackp"),
                                           fixedT0Error = fixedT0Error)
process.offlinePrimaryVertices4D = process.offlinePrimaryVertices4DnoPID.clone(TrackTimesLabel = cms.InputTag("tofPIDnoPID:t0safe"),
                                                                               TrackTimeResosLabel = cms.InputTag("tofPIDnoPID:sigmat0safe"))
process.offlinePrimaryVertices4D.TkClusParameters.TkDAClusParameters.tmerge = cms.double(0.1)
process.reconstruction_step += cms.Sequence(process.trackExtenderWithMTDnoPID * process.offlinePrimaryVertices4DnoPID * process.tofPIDnoPID * process.offlinePrimaryVertices4D)

# TOF RE-RECO
process.trackExtenderWithMTD.vtxSrc = cms.InputTag('offlinePrimaryVertices4D')
process.trackExtenderWithMTD.UseVertex = cms.bool(True) #run trackExtender using vertex constrain
process.trackExtenderWithMTD.DZCut = 0.3
process.trackExtenderWithMTD.fixedT0Error = fixedT0Error
process.tofPID.vtxsSrc = cms.InputTag('offlinePrimaryVertices4D')
process.tofPID.fixedT0Error = fixedT0Error
process.reconstruction_step += cms.Sequence(process.trackExtenderWithMTD * process.tofPID)

# DEDX RE-RECO
process.dedxPixelHarmonic2T40 = process.dedxPixelHarmonic2.clone(estimator = 'genericTruncated', fraction  = -0.4)
process.dedxPixelMeanT40 = process.dedxPixelHarmonic2.clone(estimator = 'genericTruncated', fraction  = -0.4, exponent = 1.0)
process.reconstruction_step += cms.Sequence(process.dedxPixelHarmonic2T40 * process.dedxPixelMeanT40)

# Add PbPb centrality
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
      cms.PSet(record = cms.string("HeavyIonRcd"),
                tag = cms.string("CentralityTable_HFtowers200_HydjetTuneCP5MTD_v1040mtd4x1_mc"),
                        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
                                label = cms.untracked.string("HFtowers")
                                        ),
          ])
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')
process.hiCentrality.produceHFhits = False
process.hiCentrality.produceHFtowers = True
process.hiCentrality.produceEcalhits = False
process.hiCentrality.produceZDChits = False
process.hiCentrality.produceETmidRapidity = False
process.hiCentrality.producePixelhits = False
process.hiCentrality.produceTracks = False
process.hiCentrality.producePixelTracks = False
process.hiCentrality.reUseCentrality = False
process.hiCentrality.srcReUse = cms.InputTag("hiCentrality","","RECO")
process.hiCentrality.srcTracks = cms.InputTag("generalTracks")
process.hiCentrality.srcVertex = cms.InputTag("offlinePrimaryVertices4D")
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.centralityBin.nonDefaultGlauberModel = cms.string("")
process.hiCentrality.srcEBhits = cms.InputTag("HGCalRecHit","HGCHEBRecHits")
process.hiCentrality.srcEEhits = cms.InputTag("HGCalRecHit","HGCEERecHits")

process.cent_seq = cms.Sequence(process.hiCentrality * process.centralityBin)

# Event selection filter
process.PAprimaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices4D::ANASKIM"),
    cut = cms.string("!isFake && abs(z) <= 50 && position.Rho <= 5 && tracksSize >= 2"),
    filter = cms.bool(True),   # otherwise it won't filter the events
)
process.PAcollisionEventSelection = cms.Sequence(
    process.PAprimaryVertexFilter
)
process.eventFilter = cms.Sequence(
    process.PAcollisionEventSelection
)

# Add the VertexComposite producer
from VertexCompositeAnalysis.VertexCompositeProducer.generalParticles_cff import generalParticles
process.load("VertexCompositeAnalysis.VertexCompositeProducer.generalParticles_cff")
process.D0Candidate = process.generalParticles.clone(
    primaryVertices = cms.InputTag('offlinePrimaryVertices4D::ANASKIM'),

    pdgId = cms.int32(421),
    doSwap = cms.bool(True),
    width = cms.double(0.06),

    preSelection = cms.string(""
       "charge==0"
       "&& userFloat('dauEtaDiff') <= 1.0"
       ),
    pocaSelection = cms.string(""
       "userFloat('bestMass') >= 1.8 && userFloat('bestMass') <= 1.93 "
       #"&& pt > 1.5"
       #"&& userFloat('dca') >= 0 && userFloat('dca') <= 9999."
       ),
    postSelection = cms.string(""
       "userFloat('vertexProb') >= 0.00"
       "&& userFloat('normChi2') <= 9999.0"
       ),
    finalSelection = cms.string(""
       "cos(userFloat('angle3D')) >= -2.0 && cos(userFloat('angle2D')) >= -2.0"
       #"&& abs(userFloat('angle3D')) <= 0.2"
       #"&& abs(userFloat('lVtxSig')) > 3.0"
       ),
    dedxInputs = cms.vstring('dedxPixelHarmonic2', 'dedxHarmonic2', 'dedxPixelHarmonic2T40', 'dedxPixelMeanT40'),

    mtdValueNames = cms.vstring(["tMTD", "tMTDErr", "pathLength"]),
    mtdValueLabels = cms.vstring(["trackExtenderWithMTD:generalTracktmtd:ANASKIM", "trackExtenderWithMTD:generalTracksigmatmtd:ANASKIM", "trackExtenderWithMTD:generalTrackPathLength:ANASKIM"]),

    # daughter information
    daughterInfo = cms.VPSet([
        cms.PSet(pdgId = cms.int32(321), charge = cms.int32(-1),
           selection = cms.string(
              "pt>0.7 && abs(eta)<3"
              "&& normalizedChi2<7."
              "&& quality('highPurity') && ptError/pt<0.1"
              "&& numberOfValidHits >=11"
              ),
           finalSelection = cms.string(''
              'abs(userFloat("dzSig")) < 3.0 && abs(userFloat("dxySig")) < 3.0'
              )
           ),
        cms.PSet(pdgId = cms.int32(211), charge = cms.int32(+1),
           selection = cms.string(
              "pt>0.7 && abs(eta)<3"
              "&& normalizedChi2<7."
              "&& quality('highPurity') && ptError/pt<0.1"
              "&& numberOfValidHits >=11"
              ),
           finalSelection = cms.string(''
              'abs(userFloat("dzSig")) < 3.0 && abs(userFloat("dxySig")) < 3.0'
              )
           )
    ])
)

process.BChargedCandidate = process.generalParticles.clone(
    primaryVertices = cms.InputTag('offlinePrimaryVertices4D::ANASKIM'),

    pdgId = cms.int32(521),
    doSwap = cms.bool(False),
    width = cms.double(0.32),
    mass = cms.double(5.27929),

    preSelection = cms.string(""
       "abs(charge)==1"
       ),
    pocaSelection = cms.string("userFloat('bestMass') >= 4.95 && userFloat('bestMass') <= 5.6"),
    postSelection = cms.string(""
       "userFloat('vertexProb') >= 0.00"
       "&& userFloat('normChi2') <= 9999.0"
       ),
    finalSelection = cms.string(""),
    dedxInputs = cms.vstring('dedxPixelHarmonic2', 'dedxHarmonic2', 'dedxPixelHarmonic2T40', 'dedxPixelMeanT40'),

    mtdValueNames = cms.vstring(["tMTD", "tMTDErr", "pathLength"]),
    mtdValueLabels = cms.vstring(["trackExtenderWithMTD:generalTracktmtd:ANASKIM", "trackExtenderWithMTD:generalTracksigmatmtd:ANASKIM", "trackExtenderWithMTD:generalTrackPathLength:ANASKIM"]),
    # daughter information
    daughterInfo = cms.VPSet([
        cms.PSet(pdgId = cms.int32(421), charge = cms.int32(0),
            source = cms.InputTag('D0Candidate'),
            selection = cms.string(""),
            finalSelection = cms.string('')
           ),
        cms.PSet(pdgId = cms.int32(211),
            selection = cms.string(
              "pt>0.7 && abs(eta)<3"
              "&& normalizedChi2<7."
              "&& quality('highPurity') && ptError/pt<0.1"
              "&& numberOfValidHits >=11"
              ),
           finalSelection = cms.string(''
              'abs(userFloat("dzSig")) < 3.0 && abs(userFloat("dxySig")) < 3.0'
              )
           )
    ])
)

# Define the analysis steps
#process.reconstruction_step has been setup
process.eventFilter_step = cms.Path( process.eventFilter )
process.pcentandep_step = cms.Path(process.eventFilter * process.cent_seq)
process.rereco_step = cms.Path(process.eventFilter * process.D0Candidate * process.BChargedCandidate)

# Add the VertexComposite tree
from VertexCompositeAnalysis.VertexCompositeAnalyzer.particle_tree_cff import particleAna_mc
process.bcharged_ana = particleAna_mc.clone(
    primaryVertices = cms.InputTag('offlinePrimaryVertices4D::ANASKIM'),
    recoParticles = cms.InputTag("BChargedCandidate"),
    selectEvents = cms.string("eventFilter_step"),
)

# Define the output
process.output_AOD = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
      'keep *_TriggerResults_*_*',
      'keep *_offlineBeamSpot_*_*',
      'keep *_BChargedCandidate_*_*',
      'keep *_dedx*_*_ANASKIM',
      'keep *_hiCentrality_*_*', 'keep *_centralityBin_*_*',
      'keep reco*_genParticles_*_*',
      'keep *_offlinePrimaryVertices4D*_*_ANASKIM',
      ),
    fileName = cms.untracked.string('ChargedB_BSample_NoPU.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_step')),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('AOD')
    )
)
process.output_step = cms.EndPath(process.output_AOD)

process.TFileService = cms.Service("TFileService", fileName = cms.string('bcharged_ana.root'))
process.p = cms.EndPath(process.bcharged_ana)

# Define the process schedule
process.schedule = cms.Schedule(
    process.reconstruction_step,
    process.eventFilter_step,
    process.pcentandep_step,
    process.rereco_step,
    #process.output_step,
    process.p
)

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
        #ignoreTotal = cms.untracked.int32(1)
        #)
