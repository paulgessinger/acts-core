#include "ActsExamples/EventData/SimParticle.hpp"
#include "ActsExamples/Generators/EventGenerator.hpp"
#include "ActsExamples/Generators/MultiplicityGenerators.hpp"
#include "ActsExamples/Generators/ParametricParticleGenerator.hpp"
#include "ActsExamples/Generators/VertexGenerators.hpp"
#include "ActsExamples/Propagation/PropagationAlgorithm.hpp"

#include <cmath>
#include <memory>

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace {
double thetaToEta(double theta) {
  return -1 * std::log(std::tan(theta / 2.));
}
double etaToTheta(double eta) {
  return 2 * std::atan(std::exp(-eta));
}
}  // namespace

void addGenerators(py::module_& mex) {
  py::class_<ActsExamples::GaussianVertexGenerator>(mex,
                                                    "GaussianVertexGenerator")
      .def(py::init<>())
      .def_readwrite("stddev", &ActsExamples::GaussianVertexGenerator::stddev)
      .def_readwrite("mean", &ActsExamples::GaussianVertexGenerator::mean)
      .def("__call__", &ActsExamples::GaussianVertexGenerator::operator(),
           py::is_operator());

  py::class_<ActsExamples::FixedVertexGenerator>(mex, "FixedVertexGenerator")
      .def(py::init<>())
      .def_readwrite("fixed", &ActsExamples::FixedVertexGenerator::fixed)
      .def("__call__", &ActsExamples::FixedVertexGenerator::operator(),
           py::is_operator());

  py::class_<ActsExamples::SimParticle>(mex, "SimParticle");
  py::class_<ActsExamples::SimParticleContainer>(mex, "SimParticleContainer");

  {
    using Config = ActsExamples::ParametricParticleGenerator::Config;
    auto gen = py::class_<ActsExamples::ParametricParticleGenerator>(
                   mex, "ParametricParticleGenerator")
                   .def(py::init<const Config&>())
                   .def("__call__",
                        &ActsExamples::ParametricParticleGenerator::operator(),
                        py::is_operator());

    py::class_<Config>(gen, "Config")
        .def(py::init<>())
        .def_readwrite("phiMin", &Config::phiMin)
        .def_readwrite("phiMax", &Config::phiMax)
        .def_readwrite("thetaMin", &Config::thetaMin)
        .def_readwrite("thetaMax", &Config::thetaMax)
        .def_readwrite("pMin", &Config::pMin)
        .def_readwrite("pMax", &Config::pMax)
        .def_readwrite("pTransverse", &Config::pTransverse)
        .def_readwrite("pdg", &Config::pdg)
        .def_readwrite("randomizeCharge", &Config::randomizeCharge)
        .def_readwrite("numParticles", &Config::numParticles)
        .def_property(
            "p",
            [](Config& cfg) {
              return std::pair{cfg.pMin, cfg.pMax};
            },
            [](Config& cfg, std::pair<double, double> value) {
              cfg.pMin = value.first;
              cfg.pMax = value.second;
            })
        .def_property(
            "phi",
            [](Config& cfg) {
              return std::pair{cfg.phiMin, cfg.phiMax};
            },
            [](Config& cfg, std::pair<double, double> value) {
              cfg.phiMin = value.first;
              cfg.phiMax = value.second;
            })
        .def_property(
            "theta",
            [](Config& cfg) {
              return std::pair{cfg.thetaMin, cfg.thetaMax};
            },
            [](Config& cfg, std::pair<double, double> value) {
              cfg.thetaMin = value.first;
              cfg.thetaMax = value.second;
            })
        .def_property(
            "eta",
            [](Config& cfg) {
              return std::pair{thetaToEta(cfg.thetaMin),
                               thetaToEta(cfg.thetaMax)};
            },
            [](Config& cfg, std::pair<double, double> value) {
              cfg.thetaMin = etaToTheta(value.first);
              cfg.thetaMax = etaToTheta(value.second);
            });
  }

  py::class_<ActsExamples::FixedMultiplicityGenerator>(
      mex, "FixedMultiplicityGenerator")
      .def(py::init<>())
      .def_readwrite("n", &ActsExamples::FixedMultiplicityGenerator::n)
      .def("__call__", &ActsExamples::FixedMultiplicityGenerator::operator(),
           py::is_operator());

  py::class_<ActsExamples::PoissonMultiplicityGenerator>(
      mex, "PoissonMultiplicityGenerator")
      .def(py::init<>())
      .def_readwrite("mean", &ActsExamples::PoissonMultiplicityGenerator::mean)
      .def("__call__", &ActsExamples::PoissonMultiplicityGenerator::operator(),
           py::is_operator());

  {
    using Config = ActsExamples::EventGenerator::Config;
    auto gen = py::class_<ActsExamples::EventGenerator, ActsExamples::IReader,
                          std::shared_ptr<ActsExamples::EventGenerator>>(
                   mex, "EventGenerator")
                   .def(py::init<const Config&, Acts::Logging::Level>(),
                        py::arg("config"), py::arg("level"));

    using Generator = ActsExamples::EventGenerator::Generator;
    py::class_<Generator>(gen, "Generator")
        .def(py::init<>())
        .def_readwrite("multiplicity", &Generator::multiplicity)
        .def_readwrite("vertex", &Generator::vertex)
        .def_readwrite("particles", &Generator::particles);

    py::class_<Config>(gen, "Config")
        .def(py::init<>())
        .def_readwrite("outputParticles", &Config::outputParticles)
        .def_readwrite("generators", &Config::generators)
        .def_readwrite("randomNumbers", &Config::randomNumbers);
  }
}