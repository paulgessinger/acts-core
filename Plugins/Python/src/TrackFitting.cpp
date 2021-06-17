#include "Acts/MagneticField/MagneticFieldProvider.hpp"
#include "ActsExamples/TrackFitting/SurfaceSortingAlgorithm.hpp"
#include "ActsExamples/TrackFitting/TrackFittingAlgorithm.hpp"
#include "ActsModule.hpp"

#include <memory>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

using namespace ActsExamples;
using namespace Acts;

namespace {

ACTS_PYTHON_COMPONENT(TrackFitting, ctx) {
  auto& [m, mex, prop] = ctx;

  {
    using Alg = ActsExamples::SurfaceSortingAlgorithm;
    using Config = Alg::Config;

    auto alg = py::class_<Alg, BareAlgorithm, std::shared_ptr<Alg>>(
                   mex, "SurfaceSortingAlgorithm")
                   .def(py::init<const Alg::Config&, Acts::Logging::Level>(),
                        py::arg("config"), py::arg("level"))
                   .def_property_readonly("config", &Alg::config);

    auto c = py::class_<Config>(alg, "Config").def(py::init<>());

    ACTS_PYTHON_STRUCT_BEGIN(c, Config);
    ACTS_PYTHON_MEMBER(inputProtoTracks);
    ACTS_PYTHON_MEMBER(inputSimulatedHits);
    ACTS_PYTHON_MEMBER(inputMeasurementSimHitsMap);
    ACTS_PYTHON_MEMBER(outputProtoTracks);
    ACTS_PYTHON_STRUCT_END();
  }

  {
    using Alg = ActsExamples::TrackFittingAlgorithm;
    using Config = Alg::Config;

    auto alg =
        py::class_<Alg, BareAlgorithm, std::shared_ptr<Alg>>(
            mex, "TrackFittingAlgorithm")
            .def(py::init<const Alg::Config&, Acts::Logging::Level>(),
                 py::arg("config"), py::arg("level"))
            .def_property_readonly("config", &Alg::config)
            .def_static("makeTrackFitterFunction",
                        py::overload_cast<
                            std::shared_ptr<const Acts::TrackingGeometry>,
                            std::shared_ptr<const Acts::MagneticFieldProvider>>(
                            &Alg::makeTrackFitterFunction))
            .def_static("makeTrackFitterFunction",
                        py::overload_cast<
                            std::shared_ptr<const Acts::MagneticFieldProvider>>(
                            &Alg::makeTrackFitterFunction));

    auto c = py::class_<Config>(alg, "Config").def(py::init<>());

    ACTS_PYTHON_STRUCT_BEGIN(c, Config);
    ACTS_PYTHON_MEMBER(inputMeasurements);
    ACTS_PYTHON_MEMBER(directNavigation);
    ACTS_PYTHON_MEMBER(inputSourceLinks);
    ACTS_PYTHON_MEMBER(inputProtoTracks);
    ACTS_PYTHON_MEMBER(inputInitialTrackParameters);
    ACTS_PYTHON_MEMBER(outputTrajectories);
    ACTS_PYTHON_MEMBER(fit);
    ACTS_PYTHON_MEMBER(dFit);
    ACTS_PYTHON_MEMBER(trackingGeometry);
    ACTS_PYTHON_MEMBER(multipleScattering);
    ACTS_PYTHON_MEMBER(energyLoss);
    ACTS_PYTHON_MEMBER(pickTrack);
    ACTS_PYTHON_STRUCT_END();
  }
}

}  // namespace