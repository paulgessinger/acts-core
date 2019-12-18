// This file is part of the Acts project.
//
// Copyright (C) 2019 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "Acts/Propagator/StraightLineStepper.hpp"
#include "Acts/Propagator/detail/CovarianceEngine.hpp"

namespace Acts {

void StraightLineStepper::update(State& state,
                                 const BoundParameters& pars) const {
  const auto& mom = pars.momentum();
  state.pos = pars.position();
  state.dir = mom.normalized();
  state.p = mom.norm();
  state.t = pars.time();

  if (pars.covariance()) {
    state.cov = (*(pars.covariance()));
  }
}

void StraightLineStepper::update(State& state, const Vector3D& uposition,
                                 const Vector3D& udirection, double up,
                                 double time) const {
  state.pos = uposition;
  state.dir = udirection;
  state.p = up;
  state.t = time;
}

void StraightLineStepper::covarianceTransport(State& state,
                                                      const Surface& surface) const {
  detail::covarianceTransport(state, surface);
}

}  // namespace Acts