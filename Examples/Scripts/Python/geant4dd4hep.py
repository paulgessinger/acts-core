#!/usr/bin/env python3

import acts
import acts.examples

u = acts.UnitConstants

# Preliminaries

dd4hepSvc = acts.examples.dd4hep.DD4hepGeometryService(
    xmlFileNames=["thirdparty/OpenDataDetector/xml/OpenDataDetector.xml"]
)

dd4hepG4Construction = acts.examples.DD4hepDetectorConstruction(dd4hepSvc)

g4AlgCfg = acts.examples.GeantinoRecording.Config()
g4AlgCfg.detectorConstruction = dd4hepG4Construction
g4AlgCfg.tracksPerEvent = 100

g4Alg = acts.examples.GeantinoRecording(level=acts.logging.INFO, config=g4AlgCfg)


s = acts.examples.Sequencer(events=100, numThreads=1)
s.addAlgorithm(g4Alg)

s.addWriter(
    acts.examples.RootMaterialTrackWriter(
        prePostStep=True,
        recalculateTotals=True,
        collection=g4Alg.config.outputMaterialTracks,
        filePath=g4Alg.config.outputMaterialTracks + ".root",
        level=acts.logging.INFO,
    )
)


s.run()
