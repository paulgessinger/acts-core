from pathlib import Path
import os

import pytest

from helpers import (
    geant4Enabled,
    rootEnabled,
    dd4hepEnabled,
    hepmc3Enabled,
    AssertCollectionExistsAlg,
)

pytestmark = pytest.mark.skipif(not rootEnabled, reason="ROOT not set up")


import acts
from acts.examples import Sequencer

u = acts.UnitConstants


@pytest.fixture
def field():
    return acts.ConstantBField(acts.Vector3(0, 0, 2 * u.T))


@pytest.fixture
def seq():
    return Sequencer(events=10, numThreads=1)


def assert_csv_output(csv_path, stem):
    __tracebackhide__ = True
    # print(list(csv_path.iterdir()))
    assert len([f for f in csv_path.iterdir() if f.name.endswith(stem + ".csv")]) > 0
    assert all([f.stat().st_size > 0 for f in csv_path.iterdir()])


def assert_entries(root_file, tree_name, exp):
    __tracebackhide__ = True
    import ROOT

    rf = ROOT.TFile.Open(str(root_file))
    assert rf.Get(tree_name).GetEntries() == exp, f"{root_file}:{tree_name}"


def test_fatras(trk_geo, tmp_path, field):
    from fatras import runFatras

    csv = tmp_path / "csv"
    csv.mkdir()

    nevents = 10

    root_files = [
        ("fatras_particles_final.root", "particles", nevents),
        ("fatras_particles_initial.root", "particles", nevents),
        ("hits.root", "hits", 115),
    ]

    assert len(list(csv.iterdir())) == 0
    for rf, _, _ in root_files:
        assert not (tmp_path / rf).exists()

    seq = Sequencer(events=nevents)
    runFatras(trk_geo, field, str(tmp_path), s=seq).run()

    del seq

    assert_csv_output(csv, "particles_final")
    assert_csv_output(csv, "particles_initial")
    assert_csv_output(csv, "hits")
    for f, tn, exp_entries in root_files:
        rfp = tmp_path / f
        assert rfp.exists()
        assert rfp.stat().st_size > 0

        assert_entries(rfp, tn, exp_entries)


def test_seeding(tmp_path, trk_geo, field):
    from seeding import runSeeding

    field = acts.ConstantBField(acts.Vector3(0, 0, 2 * acts.UnitConstants.T))

    csv = tmp_path / "csv"
    csv.mkdir()

    seq = Sequencer(events=10, numThreads=1)

    root_files = [
        ("estimatedparams.root", "estimatedparams", 317),
        ("performance_seeding_trees.root", "track_finder_tracks", 317),
        ("performance_seeding_hists.root", None, 0),
        ("evgen_particles.root", "particles", seq.config.events),
        ("fatras_particles_final.root", "particles", seq.config.events),
        ("fatras_particles_initial.root", "particles", seq.config.events),
    ]

    for fn, _, _ in root_files:
        fp = tmp_path / fn
        assert not fp.exists()

    assert len(list(csv.iterdir())) == 0

    runSeeding(trk_geo, field, outputDir=str(tmp_path), s=seq).run()

    del seq

    for fn, tn, exp_entries in root_files:
        fp = tmp_path / fn
        assert fp.exists()
        assert fp.stat().st_size > 0

        if tn is not None:
            assert_entries(fp, tn, exp_entries)

    assert_csv_output(csv, "evgen_particles")
    assert_csv_output(csv, "evgen_particles")
    assert_csv_output(csv, "fatras_particles_final")
    assert_csv_output(csv, "fatras_particles_initial")


@pytest.mark.slow
def test_pythia8(tmp_path, seq):
    from pythia8 import runPythia8

    (tmp_path / "csv").mkdir()

    assert not (tmp_path / "pythia8_particles.root").exists()
    assert len(list((tmp_path / "csv").iterdir())) == 0

    events = seq.config.events

    runPythia8(str(tmp_path), s=seq).run()

    del seq

    fp = tmp_path / "pythia8_particles.root"
    assert fp.exists()
    assert fp.stat().st_size > 0
    assert_entries(fp, "particles", events)

    assert len(list((tmp_path / "csv").iterdir())) > 0
    assert_csv_output(tmp_path / "csv", "particles")


def test_propagation(tmp_path, trk_geo, field, seq):
    from propagation import runPropagation

    obj = tmp_path / "obj"
    obj.mkdir()

    root_files = [("propagation_steps.root", "propagation_steps", 10000)]

    for fn, _, _ in root_files:
        fp = tmp_path / fn
        assert not fp.exists()

    assert len(list(obj.iterdir())) == 0

    runPropagation(trk_geo, field, str(tmp_path), s=seq).run()

    for fn, tn, ee in root_files:
        fp = tmp_path / fn
        assert fp.exists()
        assert fp.stat().st_size > 0
        assert_entries(fp, tn, ee)

    assert len(list(obj.iterdir())) > 0


@pytest.mark.slow
@pytest.mark.skipif(not geant4Enabled, reason="Geant4 not set up")
@pytest.mark.skipif(not dd4hepEnabled, reason="DD4hep not set up")
def test_geantino_recording(tmp_path, seq):
    from geantino_recording import runGeantinoRecording
    from acts.examples.dd4hep import DD4hepGeometryService

    root_files = [("geant-material-tracks.root", "material-tracks", 1000)]

    dd4hepSvc = acts.examples.dd4hep.DD4hepGeometryService(
        xmlFileNames=["thirdparty/OpenDataDetector/xml/OpenDataDetector.xml"]
    )
    dd4hepG4ConstructionFactory = (
        acts.examples.geant4.dd4hep.DD4hepDetectorConstructionFactory(dd4hepSvc)
    )

    for fn, _, _ in root_files:
        fp = tmp_path / fn
        assert not fp.exists()

    s = Sequencer(events=10, numThreads=1)

    runGeantinoRecording(dd4hepG4ConstructionFactory, str(tmp_path), s=s)

    s.run()

    del s

    for fn, tn, ee in root_files:
        fp = tmp_path / fn
        assert fp.exists()
        assert fp.stat().st_size > 0
        assert_entries(fp, tn, ee)


@pytest.mark.skipif(not dd4hepEnabled, reason="DD4hep not set up")
def test_truth_tracking(tmp_path):
    from truth_tracking import runTruthTracking

    from common import getOpenDataDetector

    detector, trackingGeometry, _ = getOpenDataDetector()

    field = acts.ConstantBField(acts.Vector3(0, 0, 2 * u.T))

    seq = Sequencer(events=10, numThreads=1)

    root_files = [
        ("trackstates_fitter.root", "trackstates", 9),
        ("tracksummary_fitter.root", "tracksummary", 10),
        ("performance_track_finder.root", "track_finder_tracks", 9),
        ("performance_track_fitter.root", None, -1),
    ]

    for fn, _, _ in root_files:
        fp = tmp_path / fn
        assert not fp.exists()

    runTruthTracking(trackingGeometry, field, str(tmp_path), s=seq)

    seq.run()

    del seq

    for fn, tn, ee in root_files:
        fp = tmp_path / fn
        assert fp.exists()
        assert fp.stat().st_size > 0
        if tn is not None:
            assert_entries(fp, tn, ee)


@pytest.mark.slow
@pytest.mark.skipif(not hepmc3Enabled, reason="HepMC3 plugin not available")
@pytest.mark.skipif(not dd4hepEnabled, reason="DD4hep not set up")
def test_event_recording(tmp_path, seq):
    from event_recording import runEventRecording
    from acts.examples.dd4hep import DD4hepGeometryService
    from acts.examples.hepmc3 import HepMC3AsciiReader

    out_path = tmp_path / "hepmc3"
    # out_path.mkdir()

    dd4hepSvc = acts.examples.dd4hep.DD4hepGeometryService(
        xmlFileNames=["thirdparty/OpenDataDetector/xml/OpenDataDetector.xml"]
    )
    dd4hepG4ConstructionFactory = (
        acts.examples.geant4.dd4hep.DD4hepDetectorConstructionFactory(dd4hepSvc)
    )

    s = Sequencer(events=20, numThreads=1, logLevel=acts.logging.VERBOSE)

    runEventRecording(dd4hepG4ConstructionFactory, outputDir=str(tmp_path), s=s)

    s.run()

    assert len([f for f in out_path.iterdir() if f.name.endswith("events.hepmc3")]) > 0
    assert all([f.stat().st_size > 100 for f in out_path.iterdir()])

    del s

    s = Sequencer(numThreads=1)

    s.addReader(
        HepMC3AsciiReader(
            level=acts.logging.INFO,
            inputDir=str(out_path),
            inputStem="events",
            outputEvents="hepmc-events",
        )
    )

    alg = AssertCollectionExistsAlg(
        "hepmc-tracks", name="check_alg", level=acts.logging.INFO
    )
    s.addAlgorithm(alg)

    s.run()

    assert alg.events_seen == 20
