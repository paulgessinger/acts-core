import pytest

from acts.examples import (
    TutorialVertexFinderAlgorithm,
    AdaptiveMultiVertexFinderAlgorithm,
    VertexFitterAlgorithm,
    IterativeVertexFinderAlgorithm,
    SpacePointMaker,
    TrackFindingAlgorithm,
    SeedingAlgorithm,
    TrackParamsEstimationAlgorithm,
    EventGenerator,
    FatrasAlgorithm,
    MaterialMapping,
    TruthSeedSelector,
    TruthTrackFinder,
    ParticleSelector,
    TruthVertexFinder,
    ParticleSmearing,
    TrackSelector,
    TrackFittingAlgorithm,
    SurfaceSortingAlgorithm,
    ParticlesPrinter,
    HitsPrinter,
    TrackParametersPrinter,
    PropagationAlgorithm,
    DigitizationAlgorithm,
    SmearingAlgorithm,
    PlanarSteppingAlgorithm,
)

from acts.examples.hepmc3 import HepMCProcessExtractor

from acts.examples.geant4.hepmc3 import EventRecording
from acts.examples.geant4 import GeantinoRecording


@pytest.mark.parametrize(
    "alg",
    [
        TutorialVertexFinderAlgorithm,
        AdaptiveMultiVertexFinderAlgorithm,
        VertexFitterAlgorithm,
        IterativeVertexFinderAlgorithm,
        SpacePointMaker,
        TrackFindingAlgorithm,
        SeedingAlgorithm,
        TrackParamsEstimationAlgorithm,
        EventRecording,
        HepMCProcessExtractor,
        EventGenerator,
        FatrasAlgorithm,
        MaterialMapping,
        TruthSeedSelector,
        TruthTrackFinder,
        ParticleSelector,
        TruthVertexFinder,
        ParticleSmearing,
        TrackSelector,
        TrackFittingAlgorithm,
        SurfaceSortingAlgorithm,
        ParticlesPrinter,
        HitsPrinter,
        TrackParametersPrinter,
        PropagationAlgorithm,
        GeantinoRecording,
        DigitizationAlgorithm,
        SmearingAlgorithm,
        PlanarSteppingAlgorithm,
    ],
)
def test_algorithm_interface(alg):
    assert hasattr(alg, "Config")
