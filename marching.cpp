#include "vtkActor.h"
#include "vtkActor2D.h"
#include "vtkCamera.h"
#include "vtkContourFilter.h"
#include "vtkCubeSource.h"
#include "vtkDataSetAlgorithm.h"
#include "vtkExtractEdges.h"
#include "vtkFloatArray.h"
#include "vtkGlyph3D.h"
#include "vtkIdList.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkShrinkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkThresholdPoints.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTubeFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkVectorText.h"

#include <xtgmath.h>

int main() {
  const auto scalars = vtkSmartPointer<vtkFloatArray>::New();
  scalars->InsertNextValue(1.0);
  scalars->InsertNextValue(0.0);
  scalars->InsertNextValue(0.0);
  scalars->InsertNextValue(1.0);
  scalars->InsertNextValue(0.0);
  scalars->InsertNextValue(0.0);
  scalars->InsertNextValue(0.0);
  scalars->InsertNextValue(0.0);

  const auto points = vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0, 0, 0);
  points->InsertNextPoint(1, 0, 0);
  points->InsertNextPoint(1, 1, 0);
  points->InsertNextPoint(0, 1, 0);
  points->InsertNextPoint(0, 0, 1);
  points->InsertNextPoint(1, 0, 1);
  points->InsertNextPoint(1, 1, 1);
  points->InsertNextPoint(0, 1, 1);

  const auto ids = vtkSmartPointer<vtkIdList>::New();
  ids->InsertNextId(0);
  ids->InsertNextId(1);
  ids->InsertNextId(2);
  ids->InsertNextId(3);
  ids->InsertNextId(4);
  ids->InsertNextId(5);
  ids->InsertNextId(6);
  ids->InsertNextId(7);

  const auto grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
  grid->Allocate(10, 10);
  grid->InsertNextCell(12, ids);
  grid->SetPoints(points);
  grid->GetPointData()->SetScalars(scalars);

  const auto marching = vtkSmartPointer<vtkContourFilter>::New();
  marching->SetInputData(grid);
  marching->SetValue(0, 0.5);
  marching->Update();

  const auto triangle_edges = vtkSmartPointer<vtkExtractEdges>::New();
  triangle_edges->SetInputConnection(marching->GetOutputPort());

  const auto triangle_edge_tube = vtkSmartPointer<vtkTubeFilter>::New();
  triangle_edge_tube->SetInputConnection(triangle_edges->GetOutputPort());
  triangle_edge_tube->SetRadius(0.005);
  triangle_edge_tube->SetNumberOfSides(6);
  triangle_edge_tube->UseDefaultNormalOn();
  triangle_edge_tube->SetDefaultNormal(0.577, 0.577, 0.577);

  const auto triangle_edge_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  triangle_edge_tube->SetInputConnection(triangle_edge_tube->GetOutputPort());
  triangle_edge_mapper->ScalarVisibilityOff();

  const auto triangle_edge_actor = vtkSmartPointer<vtkActor>::New();
  triangle_edge_actor->SetMapper(triangle_edge_mapper);
  const auto property = triangle_edge_actor->GetProperty();
  property->SetDiffuseColor(1, 1, 1);
  property->SetSpecular(0.4);
  property->SetSpecularPower(10);

  const auto shrink_data = vtkSmartPointer<vtkShrinkPolyData>::New();
  shrink_data->SetShrinkFactor(1);
  shrink_data->SetInputConnection(marching->GetOutputPort());

  const auto shrink_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  shrink_mapper->SetInputConnection(shrink_data->GetOutputPort());
  shrink_mapper->ScalarVisibilityOff();

  const auto shrink_actor = vtkSmartPointer<vtkActor>::New();
  shrink_actor->SetMapper(shrink_mapper);
  shrink_actor->GetProperty()->SetDiffuseColor(1, 0, 0);
  shrink_actor->GetProperty()->SetOpacity(0.6);

  const auto cube_model = vtkSmartPointer<vtkCubeSource>::New();
  cube_model->SetCenter(0.5, 0.5, 0.5);

  const auto cube_edges = vtkSmartPointer<vtkExtractEdges>::New();
  cube_edges->SetInputConnection(cube_model->GetOutputPort());

  const auto cube_filter = vtkSmartPointer<vtkTubeFilter>::New();
  cube_filter->SetInputConnection(cube_edges->GetOutputPort());
  cube_filter->SetRadius(0.01);
  cube_filter->SetNumberOfSides(6);
  cube_filter->UseDefaultNormalOn();
  cube_filter->SetDefaultNormal(0.577, 0.577, 0.577);

  const auto cube_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cube_mapper->SetInputConnection(cube_filter->GetOutputPort());

  const auto cube_actor = vtkSmartPointer<vtkActor>::New();
  cube_actor->SetMapper(cube_mapper);
  cube_actor->GetProperty()->SetDiffuseColor(0, 1, 0);
  cube_actor->GetProperty()->SetSpecular(0.4);
  cube_actor->GetProperty()->SetSpecularPower(10);

  const auto sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetRadius(0.04);
  sphere->SetPhiResolution(20);
  sphere->SetThetaResolution(20);

  const auto threshold = vtkSmartPointer<vtkThresholdPoints>::New();
  threshold->SetInputData(grid);
  threshold->ThresholdByUpper(0.5);

  const auto vertices = vtkSmartPointer<vtkGlyph3D>::New();
  vertices->SetInputConnection(threshold->GetOutputPort());
  vertices->SetSourceConnection(sphere->GetOutputPort());

  const auto sphere_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphere_mapper->SetInputConnection(vertices->GetOutputPort());
  sphere_mapper->ScalarVisibilityOff();

  const auto sphere_actor = vtkSmartPointer<vtkActor>::New();
  sphere_actor->SetMapper(sphere_mapper);
  sphere_actor->GetProperty()->SetDiffuseColor(0, 0, 1);

  const auto case_label = vtkSmartPointer<vtkVectorText>::New();
  case_label->SetText("Case 1");

  const auto label_transform = vtkSmartPointer<vtkTransform>::New();
  label_transform->Identity();
  label_transform->Translate(-0.2, 0, 1.25);
  label_transform->Scale(0.05, 0.05, 0.05);

  const auto label_filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  label_filter->SetTransform(label_transform);
  label_filter->SetInputConnection(case_label->GetOutputPort());

  const auto label_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  label_mapper->SetInputConnection(label_filter->GetOutputPort());

  const auto label_actor = vtkSmartPointer<vtkActor>::New();
  label_actor->SetMapper(label_mapper);

  const auto base = vtkSmartPointer<vtkCubeSource>::New();
  base->SetXLength(1.5);
  base->SetYLength(0.01);
  base->SetZLength(1.5);

  const auto base_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  base_mapper->SetInputConnection(base->GetOutputPort());

  const auto base_actor = vtkSmartPointer<vtkActor>::New();
  base_actor->SetMapper(base_mapper);
  base_actor->SetPosition(0.5, -0.09, 0.5);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(triangle_edge_actor);
  render->AddActor(base_actor);
  render->AddActor(label_actor);
  render->AddActor(shrink_actor);
  render->AddActor(sphere_actor);
  render->AddActor(cube_actor);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->SetSize(640, 480);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  scalars->InsertValue(0, 1);
  scalars->InsertValue(1, 0);
  scalars->InsertValue(2, 1);
  scalars->InsertValue(3, 0);
  scalars->InsertValue(4, 0);
  scalars->InsertValue(5, 0);
  scalars->InsertValue(6, 1);
  scalars->InsertValue(7, 1);

  case_label->SetText("case 12 - 11000101");

  grid->Modified();

  render->ResetCamera();
  render->GetActiveCamera()->Dolly(1.2);
  render->GetActiveCamera()->Azimuth(30);
  render->GetActiveCamera()->Elevation(20);
  render->ResetCameraClippingRange();

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}