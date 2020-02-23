#include "vtkMatrix4x4.h"
#include "vtkNew.h"
#include "vtkPerspectiveTransform.h"
#include "vtkTransform.h"

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

auto main(int argc, char** argv) -> int {
  vtkNew<vtkMatrix4x4> matrix;
  matrix->SetElement(0, 0, 1);
  matrix->SetElement(0, 1, 2);
  matrix->SetElement(0, 2, 3);
  matrix->SetElement(0, 3, 4);
  matrix->SetElement(1, 0, 2);
  matrix->SetElement(1, 1, 2);
  matrix->SetElement(1, 2, 3);
  matrix->SetElement(1, 3, 4);
  matrix->SetElement(2, 0, 3);
  matrix->SetElement(2, 1, 2);
  matrix->SetElement(2, 2, 3);
  matrix->SetElement(2, 3, 4);
  matrix->SetElement(3, 0, 4);
  matrix->SetElement(3, 1, 2);
  matrix->SetElement(3, 2, 3);
  matrix->SetElement(3, 3, 4);

  vtkNew<vtkPerspectiveTransform> perspective_transform;
  perspective_transform->SetMatrix(matrix);

  vtkNew<vtkTransform> transfrom;
  transfrom->SetMatrix(matrix);

  pointType p;
  p[0] = 1.0;
  p[1] = 2.0;
  p[2] = 3.0;

  pointType normal_projection;
  transfrom->TransformPoint(p.data(), normal_projection.data());
  std::cout << "Standard projection: " << normal_projection << std::endl;

  pointType perspectiveProjection;
  perspective_transform->TransformPoint(p.data(), perspectiveProjection.data());
  std::cout << "Perspective projection: " << perspectiveProjection << std::endl;

  return 0;
}