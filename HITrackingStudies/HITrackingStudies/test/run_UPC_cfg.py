### Options to run
'''
n=integer number of events
i=input file path(s) (if local, must use 'file:/path/file.root')
t=.txt file of input file path(s) (if local, must use 'file:/path/file.root')
o=output file path
(UNUSED)sample="MC_RecoDebug","MC_Reco_AOD","MC_MiniAOD","Data_Reco_AOD","Data_MiniAOD"

To run it, please, do e.g.:
    cmsRun run_UPC_cfg.py n=1000 i=dir/input_file.root o=dir/output_file.root
'''

### DEFAULTS / OVERWRITTEN BY FLAGGED ARGS ###

# Command Line Arguments
defInput     = 'tempInput/trk_GeneralTrack.root'
defOutput    = 'tempOutput/trk_GeneralTrack.root'
defNEvent    = -1
# Other Variables
defFileServ  = 'trk_GeneralTrack.root'
defGlobalTag = '132X_mcRun3_2023_realistic_HI_v7'

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
process.load('HITrackingStudies.HITrackingStudies.HITrackCorrectionAnalyzer_cfi')

# Parse arguments
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing('analysis')
options.register(
    'n',
    defNEvent,                          # default value
    VarParsing.multiplicity.singleton,  # singleton or list
    VarParsing.varType.int,             # string, int, bool or float
    "Max number of events"
)
options.register(
    'i',
    defInput,                           # default value
    VarParsing.multiplicity.list,       # singleton or list
    VarParsing.varType.string,          # string, int, bool or float
    "Input file path"
)
options.register(
    'o',
    defOutput,                          # default value
    VarParsing.multiplicity.list,       # singleton or list
    VarParsing.varType.string,          # string, int, bool or float
    "Output file path"
)
options.register(
    't',
    '',                                 # default value
    VarParsing.multiplicity.singleton,  # singleton or list
    VarParsing.varType.string,          # string, int, bool or float
    ".txt file with list of inputs"
)
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
    fileName = cms.string(defFileServ)
)

# Set input source
inputList = []
if( options.t ):
    inputList = open(options.t).readlines()
else:
    inputList = options.i
process.source = cms.Source(
    "PoolSource",
    duplicateCheckMode  = cms.untracked.string("noDuplicateCheck"),
    fileNames           = cms.untracked.vstring(inputList),
    skipEvents          = cms.untracked.uint32(0),
    #secondaryFileNames  = cms.untracked.vstring()
)

### TRACK CUTS ###

# Input collections
process.HITrackCorrections.centralitySrc    = cms.InputTag("")
process.HITrackCorrections.trackSrc         = cms.InputTag("generalTracks")
process.HITrackCorrections.vertexSrc        = cms.InputTag("offlinePrimaryVertices")
process.HITrackCorrections.qualityString    = cms.string("highPurity") # "Balanced" track eff + bkg
process.HITrackCorrections.pfCandSrc        = cms.InputTag("particleFlow") # Particle Flow candidates
process.HITrackCorrections.jetSrc           = cms.InputTag("ak4CaloJets")
process.HITrackCorrections.associatorMap    = cms.InputTag("trackingParticleRecoTrackAsssociation")

# Cut options
# ??? Why are there no cuts ???
process.HITrackCorrections.useCentrality    = False
process.HITrackCorrections.applyTrackCuts   = False
process.HITrackCorrections.fillNTuples      = False
process.HITrackCorrections.applyVertexZCut  = False
process.HITrackCorrections.doVtxReweighting = False
process.HITrackCorrections.doCaloMatched    = False

# Cut values
process.HITrackCorrections.dxyErrMax    = 3.0
process.HITrackCorrections.dzErrMax     = 3.0
process.HITrackCorrections.ptErrMax     = 0.1
process.HITrackCorrections.nhitsMin     = 11
process.HITrackCorrections.chi2nMax     = 0.18
process.HITrackCorrections.reso         = 0.5
process.HITrackCorrections.crossSection = 1.0 #1.0 is no reweigh

# Algo Parameters
process.HITrackCorrections.algoParameters = cms.vint32(
    0,1,2,3,4,5,6,7,8,9,
    10,11,12,13,14,15,16,17,18,19,
    20,21,22,23,24,25,26,27,28,29,
    30,31,32,33,34,35,36,37,38,39,
    40,41,42,43,44,45,46
)
# Vertex reweight parameters
process.HITrackCorrections.vtxWeightParameters = cms.vdouble(
    0.0306789,
    0.427748,
    5.16555,
    0.0228019,
    -0.02049,
    7.01258
)

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(
    process.GlobalTag,
    defGlobalTag
)

# Forest style analyzers (anaTrack module)
# (not affected by HITrackCorrections code)
process.load('HITrackingStudies.AnalyzerCode.trackAnalyzer_cff')
process.anaTrack.associatorMap      = cms.InputTag("trackingParticleRecoTrackAsssociation")
process.anaTrack.useCentrality      = False
process.anaTrack.trackSrc           = 'generalTracks'
process.anaTrack.mvaSrc             = cms.InputTag("generalTracks","MVAValues")
process.anaTrack.doMVA              = False # Why do we have the above line if no MVA?
process.anaTrack.doSimTrack         = True
process.anaTrack.doSimVertex        = True
process.anaTrack.fillSimTrack       = True
process.anaTrack.doPFMatching       = False
process.anaTrack.doHighestPtVertex  = False
process.anaTrack.doTrackVtxWImpPar  = False

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
        process.HITrackCorrections *
        process.anaTrack
)
