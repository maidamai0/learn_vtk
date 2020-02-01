#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkDataArray.h"
#include "vtkDataObject.h"
#include "vtkOutlineFilter.h"
#include "vtkPointData.h"
#include "vtkPointSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRungeKutta4.h"
#include "vtkSmartPointer.h"
#include "vtkStreamTracer.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkStructuredGridReader.h"

#include "platform.h"
#include "vtkTubeFilter.h"
#include <filesystem>

int main() {
  const auto reader = vtkSmartPointer<vtkStructuredGridReader>::New();
  reader->SetFileName(
    (fs::canonical("Data").string() + "/office.binary.vtk").c_str());
  reader->Update();

  const auto length = reader->GetOutput()->GetLength();

  const auto max_velocity
    = reader->GetOutput()->GetPointData()->GetVectors()->GetMaxNorm();
  const auto max_time = 35.0 * length / max_velocity;

  const auto seeds = vtkSmartPointer<vtkPointSource>::New();
  seeds->SetRadius(0.15);
  seeds->SetCenter(0.1, 2.1, 0.5);
  seeds->SetNumberOfPoints(10);

  const auto integration = vtkSmartPointer<vtkRungeKutta4>::New();

  const auto stream_tracer = vtkSmartPointer<vtkStreamTracer>::New();
  stream_tracer->SetInputConnection(reader->GetOutputPort());
  stream_tracer->SetSourceConnection(seeds->GetOutputPort());
  stream_tracer->SetStartPosition(0.1, 2.1, 0.5);
  stream_tracer->SetMaximumPropagation(500);
  stream_tracer->SetInitialIntegrationStep(0.05);
  stream_tracer->SetIntegrationDirectionToBoth();
  stream_tracer->SetIntegrator(integration);

  const auto stream_tube = vtkSmartPointer<vtkTubeFilter>::New();
  stream_tube->SetInputConnection(stream_tracer->GetOutputPort());
  stream_tube->SetInputArrayToProcess(
    1, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "vectors");
  stream_tube->SetRadius(0.02);
  stream_tube->SetNumberOfSides(12);
  stream_tube->SetVaryRadiusToVaryRadiusByVector();

  const auto stream_tube_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  stream_tube_mapper->SetInputConnection(stream_tube->GetOutputPort());
  stream_tube_mapper->SetScalarRange(
    reader->GetOutput()->GetPointData()->GetScalars()->GetRange());

  const auto stream_tube_actor = vtkSmartPointer<vtkActor>::New();
  stream_tube_actor->SetMapper(stream_tube_mapper);
  stream_tube_actor->GetProperty()->BackfaceCullingOn();

  const auto table = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  table->SetInputConnection(reader->GetOutputPort());
  table->SetExtent(11, 15, 7, 9, 8, 8);

  const auto mapper_table = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper_table->SetInputConnection(table->GetOutputPort());
  mapper_table->ScalarVisibilityOff();

  const auto actor_table = vtkSmartPointer<vtkActor>::New();
  actor_table->SetMapper(mapper_table);
  actor_table->GetProperty()->SetColor(0.59, 0.427, 0.392);

  const auto table_1 = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  table_1->SetInputConnection(reader->GetOutputPort());
  table_1->SetExtent(11, 15, 10, 12, 8, 8);

  const auto mapper_table_1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper_table_1->SetInputConnection(table_1->GetOutputPort());
  mapper_table_1->ScalarVisibilityOff();

  const auto actor_table_1 = vtkSmartPointer<vtkActor>::New();
  actor_table_1->SetMapper(mapper_table_1);
  actor_table_1->GetProperty()->SetColor(0.59, 0.427, 0.392);

  const auto filling_cabinet
    = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  filling_cabinet->SetInputConnection(reader->GetOutputPort());
  filling_cabinet->SetExtent(15, 15, 7, 9, 8, 8);

  const auto mapper_cabinet = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper_cabinet->SetInputConnection(filling_cabinet->GetOutputPort());
  mapper_cabinet->ScalarVisibilityOff();

  const auto actor_cabinet = vtkSmartPointer<vtkActor>::New();
  actor_cabinet->SetMapper(mapper_cabinet);
  actor_cabinet->GetProperty()->SetColor(0.8, 0.8, 0.6);

  const auto filling_cabinet_1
    = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  filling_cabinet_1->SetInputConnection(reader->GetOutputPort());
  filling_cabinet_1->SetExtent(15, 15, 10, 12, 8, 8);

  const auto mapper_cabinet_1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper_cabinet_1->SetInputConnection(filling_cabinet_1->GetOutputPort());
  mapper_cabinet_1->ScalarVisibilityOff();

  const auto actor_cabinet_1 = vtkSmartPointer<vtkActor>::New();
  actor_cabinet_1->SetMapper(mapper_cabinet_1);
  actor_cabinet_1->GetProperty()->SetColor(0.8, 0.8, 0.6);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputConnection(reader->GetOutputPort());

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(stream_tube_actor);
  render->AddActor(actor_table_1);
  render->AddActor(actor_table);
  render->AddActor(actor_cabinet);
  render->AddActor(actor_cabinet_1);
  render->AddActor(outline_actor);
  render->SetBackground(0.5, 0.5, 0.5);

  const auto camera = vtkSmartPointer<vtkCamera>::New();
  camera->SetClippingRange(0.726079, 36.3039);
  camera->SetFocalPoint(2.43584, 2.15046, 1.11104);
  camera->SetPosition(-4.76183, -10.4426, 0.989827);
  camera->SetViewUp(0.0511273, 0.132773, 0.989827);
  camera->SetViewAngle(18.604);
  camera->Zoom(1.2);
  render->SetActiveCamera(camera);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render_window->Render();
  interactor->Initialize();
  interactor->Start();
}