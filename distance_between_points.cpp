/**
 * @file distance_between_points.cpp
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief calculate the distance between two points
 * @version 0.1
 * @date 2020-02-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "vtkMath.h"

#include "cassert"
#include <array>
#include <iostream>
#include <ostream>

using pointType = std::array<double, 3>;

auto operator<<(std::ostream& os, const pointType& p) -> std::ostream& {
  os << "[" << p[0];
  for (std::size_t i = 1; i < p.size(); ++i) {
    os << "," << p[i];
  }
  os << "]";

  return os;
}

auto distance(const pointType& p1, const pointType& p2) {
  const auto d = sqrt(vtkMath::Distance2BetweenPoints(p1.data(), p2.data()));
  std::cout << "distance of " << p1 << " and " << p2 << " is " << d
            << std::endl;
  return d;
}

auto main(int argc, char** atgv) -> int {
  pointType p0{0.0, 0.0, 0.0};
  pointType p1{3.0, 0.0, 0.0};
  pointType p2{0.0, 4.0, 0.0};

  distance(p0, p1);
  distance(p1, p2);
  distance(p0, p2);
}