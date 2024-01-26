### Options to run
'''
sample="MC_RecoDebug","MC_Reco_AOD","MC_MiniAOD","Data_Reco_AOD","Data_MiniAOD"
n=integer number of events

To run it, please, do e.g.:
    cmsRun run_UPC_cfg.py n=1000 i='trk_GeneralTrack.root' o='trk_GeneralTrack.root'
    cmsRun run_PbPb_cfg.py sample="Data_Reco_AOD" n=100 runOverStreams=False >& OutPut.txt &

'''

### SETUP ###

import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TRACKANA', eras.Run3_2023_UPC)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('UPCTrackingStudies.UPCTrackCorrectionAnalyzer_cfi')

# Parse arguments
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('analysis')
options.register (
    'n',
    -1, # default value
    VarParsing.VarParsing.multiplicity.singleton,   # singleton or list
    VarParsing.VarParsing.varType.int,              # string, int, bool or float
    "n")
options.register (
    'i',
    'tempInput/trk_GeneralTrack.root', # default value
    VarParsing.VarParsing.multiplicity.singleton,   # singleton or list
    VarParsing.VarParsing.varType.string,           # string, int, bool or float
    "i")
options.register (
    'o',
    'tempOutput/trk_GeneralTrack.root', # default value
    VarParsing.VarParsing.multiplicity.singleton,   # singleton or list
    VarParsing.VarParsing.varType.string,           # string, int, bool or float
    "o")
options.parseArguments()

# Process arguments
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.n)
)
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
)
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string(options.o) # SET OUTPUT FILE?????
)
# Set input source
process.source = cms.Source(
    "PoolSource",
    duplicateCheckMode  = cms.untracked.string("noDuplicateCheck"),
    fileNames           = cms.untracked.vstring('file:' + options.i),
    skipEvents          = cms.untracked.uint32(0),
    secondaryFileNames  = cms.untracked.vstring()
)

### TRACK CUTS ###

# Input collections
process.UPCTrackCorrections.centralitySrc = cms.InputTag("")
process.UPCTrackCorrections.trackSrc      = cms.InputTag("generalTracks")
process.UPCTrackCorrections.vertexSrc     = cms.InputTag("offlinePrimaryVertices")
process.UPCTrackCorrections.qualityString = cms.string("highPurity")
process.UPCTrackCorrections.pfCandSrc     = cms.InputTag("particleFlow")
process.UPCTrackCorrections.jetSrc        = cms.InputTag("ak4CaloJets")
process.UPCTrackCorrections.associatorMap = cms.InputTag("trackingParticleRecoTrackAsssociation")

# Cut options
process.UPCTrackCorrections.useCentrality    = False
process.UPCTrackCorrections.applyTrackCuts   = False
process.UPCTrackCorrections.fillNTuples      = False
process.UPCTrackCorrections.applyVertexZCut  = False
process.UPCTrackCorrections.doVtxReweighting = False
process.UPCTrackCorrections.doCaloMatched    = False

# Cut values
process.UPCTrackCorrections.dxyErrMax = 3.0
process.UPCTrackCorrections.dzErrMax  = 3.0
process.UPCTrackCorrections.ptErrMax  = 0.1
process.UPCTrackCorrections.nhitsMin  = 11
process.UPCTrackCorrections.chi2nMax  = 0.18
process.UPCTrackCorrections.reso      = 0.5
#process.UPCTrackCorrections.crossSection = 1.0 #1.0 is no reweigh

#algo
process.UPCTrackCorrections.algoParameters = cms.vint32(
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46
)
# vertex reweight parameters
process.UPCTrackCorrections.vtxWeightParameters = cms.vdouble(
    0.0306789, # WHAT DO THESE MEAN????
    0.427748,
    5.16555,
    0.0228019,
    -0.02049,
    7.01258
)

### WHAT IS THIS?????
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    '132X_mcRun3_2023_realistic_HI_v7'
)
###

# Forest style analyzers (anaTrack module)
# (not affected by UPCTrackCorrections code)
process.load('UPCTrackingStudies.AnalyzerCode.trackAnalyzer_cff')
process.anaTrack.associatorMap     = cms.InputTag("trackingParticleRecoTrackAsssociation")
process.anaTrack.useCentrality     = False
process.anaTrack.trackSrc          = 'generalTracks'
process.anaTrack.mvaSrc            = cms.InputTag("generalTracks","MVAValues")
process.anaTrack.doMVA             = False
process.anaTrack.doSimTrack        = True
process.anaTrack.doSimVertex       = True
process.anaTrack.fillSimTrack      = True
process.anaTrack.doPFMatching      = False
process.anaTrack.doHighestPtVertex = False
process.anaTrack.doTrackVtxWImpPar = False

process.load('SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi')
process.trackingParticleRecoTrackAsssociation.label_tr = cms.InputTag("generalTracks")

process.load('SimTracker.TrackAssociatorProducers.quickTrackAssociatorByHits_cfi')
process.quickTrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')

process.load('SimTracker.TrackerHitAssociation.tpClusterProducer_cfi')

process.simRecoTrackAssocSeq = cms.Sequence(
    process.tpClusterProducer *
    process.quickTrackAssociatorByHits *
    process.trackingParticleRecoTrackAsssociation
)

process.p = cms.Path(
    process.simRecoTrackAssocSeq *
    process.UPCTrackCorrections *
    process.anaTrack
)
