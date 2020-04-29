#include "fmt/core.h"

#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkIntArray.h"
#include "vtkNew.h"
#include "vtkPolyData.h"
#include "vtkSphereSource.h"

#include <array>
#include <iostream>
#include <ostream>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkSphereSource> sphere;
  vtkNew<vtkPolyData> polydata;
  polydata->ShallowCopy(sphere->GetOutput());

  vtkNew<vtkDoubleArray> double_data;
  double_data->SetNumberOfComponents(3);
  double_data->SetName("DoubleArray");
  std::array<double, 3> double_value{0, 0, 0};
  double_data->InsertNextTypedTuple(double_value.data());
  //   double_data->InsertNextTuple3(0.1, 0.2, 0.3); // this is ok too.

  polydata->GetFieldData()->AddArray(double_data);

  vtkNew<vtkIntArray> int_data;
  int_data->SetName("IntData");
  int_data->SetNumberOfComponents(1);
  int_data->InsertNextValue(5);
  polydata->GetFieldData()->AddArray(int_data);

  // retrive by index
  const auto retrived_double_data = dynamic_cast<vtkDoubleArray*>(
    polydata->GetFieldData()->GetAbstractArray(0));

  fmt::print("double value is [{},{},{}]\n", retrived_double_data->GetValue(0),
             retrived_double_data->GetValue(1),
             retrived_double_data->GetValue(2));

  // retrive by name
  const auto retrived_int_data = dynamic_cast<vtkIntArray*>(
    polydata->GetFieldData()->GetAbstractArray("IntData"));
  fmt::print("int value is {}\n", retrived_int_data->GetValue(0));
}