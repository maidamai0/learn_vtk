#include "vtkLine.h"
#include "vtkNew.h"
#include "vtkPoints.h"
#include <array>
#include <iostream>

using pointType = std::array<double, 3>;

auto operator<<(std::ostream& os, const pointType& p) -> std::ostream& {
  os << "[" << p[0];
  for (std::size_t i = 1; i < p.size(); ++i) {
    os << "," << p[i];
  }
  os << "]";

  return os;
}

auto distance(const pointType& p, const pointType& line_p0,
              const pointType& line_p1) {
  std::cout << "line passes " << line_p0 << " and " << line_p1 << std::endl;

  double t = 0;
  pointType closest{0.0, 0.0, 0.0};
  const auto d = vtkLine::DistanceToLine(p.data(), line_p0.data(),
                                         line_p1.data(), t, closest.data());
  std::cout << "distance from point " << p << " to line is " << d
            << " , the closest point is " << closest << std::endl;
}

auto main(int argc, char** argv) -> int {
  const pointType line_p0{0.0, 1.0, 0.0};
  const pointType line_p1{1.0, 0.0, 0.0};

  const pointType p0{0.0, 0.0, 0.0};
  const pointType p1{0.0, 0.5, 0.0};
  distance(p0, line_p0, line_p1);
  distance(p1, line_p0, line_p1);
}