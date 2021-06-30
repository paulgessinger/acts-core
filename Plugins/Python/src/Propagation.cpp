#include "Acts/Plugins/Python/Utilities.hpp"
#include "Acts/Propagator/AtlasStepper.hpp"
#include "Acts/Propagator/EigenStepper.hpp"
#include "Acts/Propagator/Navigator.hpp"
#include "Acts/Propagator/Propagator.hpp"
#include "Acts/Propagator/StraightLineStepper.hpp"
#include "ActsExamples/Propagation/PropagationAlgorithm.hpp"

#include <memory>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace {
template <typename stepper_t>
void addStepper(const std::string& prefix, py::module_& m, py::module_& prop,
                py::module_& mex) {
  auto stepper = py::class_<stepper_t>(m, (prefix + "Stepper").c_str());

  if constexpr (std::is_same_v<stepper_t, Acts::StraightLineStepper>) {
    stepper.def(py::init<>());
  } else {
    stepper.def(py::init<std::shared_ptr<const Acts::MagneticFieldProvider>>());
  }

  using propagator_t = Acts::Propagator<stepper_t, Acts::Navigator>;
  py::class_<propagator_t>(prop, (prefix + "Propagator").c_str())
      .def(py::init<stepper_t, Acts::Navigator>());

  using Algorithm = typename ActsExamples::PropagationAlgorithm<propagator_t>;
  using Config = typename Algorithm::Config;
  auto alg = py::class_<Algorithm, ActsExamples::BareAlgorithm,
                        std::shared_ptr<Algorithm>>(
                 mex, (prefix + "PropagationAlgorithm").c_str())
                 .def(py::init<const Config&, Acts::Logging::Level>());

  auto c = py::class_<Config>(alg, "Config").def(py::init<propagator_t>());
  ACTS_PYTHON_STRUCT_BEGIN(c, Config);
  ACTS_PYTHON_MEMBER(propagator);
  ACTS_PYTHON_MEMBER(randomNumberSvc);
  ACTS_PYTHON_MEMBER(mode);
  ACTS_PYTHON_MEMBER(sterileLogger);
  ACTS_PYTHON_MEMBER(debugOutput);
  ACTS_PYTHON_MEMBER(energyLoss);
  ACTS_PYTHON_MEMBER(multipleScattering);
  ACTS_PYTHON_MEMBER(recordMaterialInteractions);
  ACTS_PYTHON_MEMBER(ntests);
  ACTS_PYTHON_MEMBER(d0Sigma);
  ACTS_PYTHON_MEMBER(z0Sigma);
  ACTS_PYTHON_MEMBER(phiSigma);
  ACTS_PYTHON_MEMBER(thetaSigma);
  ACTS_PYTHON_MEMBER(qpSigma);
  ACTS_PYTHON_MEMBER(tSigma);
  ACTS_PYTHON_MEMBER(phiRange);
  ACTS_PYTHON_MEMBER(etaRange);
  ACTS_PYTHON_MEMBER(ptRange);
  ACTS_PYTHON_MEMBER(ptLoopers);
  ACTS_PYTHON_MEMBER(maxStepSize);
  ACTS_PYTHON_MEMBER(propagationStepCollection);
  ACTS_PYTHON_MEMBER(propagationMaterialCollection);
  ACTS_PYTHON_MEMBER(covarianceTransport);
  ACTS_PYTHON_MEMBER(covariances);
  ACTS_PYTHON_MEMBER(correlations);
  ACTS_PYTHON_STRUCT_END();
}

}  // namespace

namespace Acts::Python {
void addPropagation(Context& ctx) {
  auto [m, prop, mex] = ctx.get("main", "propagation", "examples");

  using Config = Acts::Navigator::Config;
  auto nav = py::class_<Acts::Navigator, std::shared_ptr<Acts::Navigator>>(
                 m, "Navigator")
                 .def(py::init<Config>());

  auto c = py::class_<Config>(nav, "Config").def(py::init<>());

  ACTS_PYTHON_STRUCT_BEGIN(c, Config);
  ACTS_PYTHON_MEMBER(resolveMaterial);
  ACTS_PYTHON_MEMBER(resolvePassive);
  ACTS_PYTHON_MEMBER(resolveSensitive);
  ACTS_PYTHON_MEMBER(trackingGeometry);
  ACTS_PYTHON_STRUCT_END();

  addStepper<Acts::EigenStepper<>>("Eigen", m, prop, mex);
  addStepper<Acts::AtlasStepper>("Atlas", m, prop, mex);
  addStepper<Acts::StraightLineStepper>("StraightLine", m, prop, mex);
}

}  // namespace Acts::Python