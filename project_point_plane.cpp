#include "platform.h"

#include "vtkNew.h"
#include "vtkPlane.h"
#include <iostream>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkPlane> plane;
  // plane->SetOrigin(0.0, 0.0, 0.0);
  // plane->SetNormal(0.0, 0.0, 1.0);

  pointType p{23.1, 54.6, 9.2};
  pointType origin{0.0, 0.0, 0.0};
  pointType normal{0.0, 0.0, 1.0};
  pointType projected;

  plane->ProjectPoint(p.data(), origin.data(), normal.data(), projected.data());
  std::cout << "Projected: " << projected << std::endl;
}