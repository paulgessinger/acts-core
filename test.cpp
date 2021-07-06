#include "Acts/Surfaces/AnnulusBounds.hpp"
#include "Acts/Utilities/Helpers.hpp"

#include <iostream>

using namespace Acts;

int main() {
  // double phi = 0.198263;
  // double phiS = 0.02;
  // double R0 = 609.405;
  // double maxR = 637.209;
  // double minR = 575.594;
  // Acts::Vector2 O0(R0 * (1 - std::cos(phiS)), R0 * std::sin(-phiS));

  // std::cout << "i,vx1,vy1,vx2,vy2,vx3,vy3,vx4,vy4\n";
  // std::cout << "0";
  // for (auto v : bnd.vertices(0)) {
  //   std::cout << "," << v[0] << "," << v[1];
  // }
  // std::cout << "\n" << std::endl;
  auto p = [](unsigned int i, auto bnd, double R0) {
    for (auto v : bnd.vertices(0)) {
      std::cout << i << "," << v[0] << "," << v[1];
      std::cout << "," << bnd.moduleOrigin()[0] << "," << bnd.moduleOrigin()[1];
      std::cout << "," << R0;
      std::cout << "," << bnd.get(AnnulusBounds::eMinR) << ","
                << bnd.get(AnnulusBounds::eMaxR);

      std::cout << std::endl;
    }
  };
  // auto bnd = AnnulusBounds{minR, maxR, -phi / 2.0, phi / 2.0, O0, 0};

  std::cout << "i,x,y,Ox,Oy,R0,minR,maxR" << std::endl;
  std::cout << std::setprecision(10);
  // p(0, bnd);

  // unsigned int i = 1;

  // double dR = maxR - minR;
  // auto var = [&](double f) {
  //   double R = (minR + (minR + dR * f)) / 2.0;
  //   Acts::Vector2 O(R * (1 - std::cos(phiS)), R * std::sin(-phiS));
  //   p(i++, AnnulusBounds{minR, minR + dR * f, -phi / 2.0, phi / 2.0, O, 0});
  // };

  // // var(0.25);
  // // var(0.5);
  // // var(0.75);
  // var(1.0);

  std::vector<std::array<double, 5>> vals = {
      {0.19772, 0.02, 438.614, 488.423, 384.5},  // big module
      {0.19772, 0.02, 393.99, 403.481, 384.5},
      // {0.19772, 0.02, 393.99, 403.481, 384.5},
      {0.19772, 0.02, 415.471, 427.462, 403.481},
      // {0.19772, 0.02, 415.471, 427.462, 403.481},
      {0.197785, 0.02, 441.952, 456.442, 427.462},
      // {0.197785, 0.02, 441.952, 456.442, 427.462},
      {0.197785, 0.02, 472.433, 488.423, 456.442},
      // {0.197785, 0.02, 472.433, 488.423, 456.442},
      // {0.198032, 0.02, 498.87, 507.916, 489.823},
      // {0.198032, 0.02, 498.87, 507.916, 489.823},
      // {0.198032, 0.02, 521.462, 535.009, 507.916},
      // {0.198032, 0.02, 521.462, 535.009, 507.916},
      // {0.198074, 0.02, 547.055, 559.101, 535.009},
      // {0.198074, 0.02, 547.055, 559.101, 535.009},
      // {0.198074, 0.02, 566.648, 574.194, 559.101},
      // {0.198074, 0.02, 566.648, 574.194, 559.101},
      // {0.198263, 0.02, 590.998, 606.402, 575.594},
      // {0.198263, 0.02, 590.998, 606.402, 575.594},
      // {0.198263, 0.02, 621.805, 637.209, 606.402},
      // {0.198263, 0.02, 621.805, 637.209, 606.402},
  };

  auto [bphi, bphiS, bR0, bmaxR, bminR] = vals[0];
  Acts::Vector2 bO0(bR0 * (1 - std::cos(bphiS)), bR0 * std::sin(-bphiS));

  unsigned int i = 0;
  for (auto& val : vals) {
    auto [phi, phiS, R0, maxR, minR] = val;
    // Acts::Vector2 O0(R0 * (1 - std::cos(phiS)), R0 * std::sin(-phiS));

    std::cout << R0 << " <-> " << bR0 << std::endl;

    auto bnd = AnnulusBounds{minR, maxR, -phi / 2.0, phi / 2.0, bO0, 0};
    p(i++, bnd, R0);
  }
}