#include "fmt/core.h"

#include "vtkActor.h"
#include "vtkAreaPicker.h"
#include "vtkCallbackCommand.h"
#include "vtkCellArray.h"
#include "vtkCommand.h"
#include "vtkInteractorStyleRubberBandPick.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkObject.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProp3DCollection.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkType.h"

#include <array>
#include <iostream>

void pick_callback_function(vtkObject* caller,
                            long unsigned int vtkNotUsed(eventId),
                            void* vtkNotUsed(client_data),
                            void* vtkNotUsed(call_data)) {
  // fmt::print("Pick {}");
  std::cout << __FUNCTION__ << std::endl;
  auto picker = static_cast<vtkAreaPicker*>(caller);
  auto props = picker->GetProp3Ds();
  props->InitTraversal();

  for (vtkIdType i = 0; i < props->GetNumberOfItems(); ++i) {
    auto prop = props->GetNextProp3D();
    std::cout << "picked prop:" << prop << std::endl;
    prop->Print(std::cout);
  }
}

auto main(int argc, char** argv) -> int {
  vtkNew<vtkPoints> points;
  vtkNew<vtkCellArray> vertices;

  std::array<vtkIdType, 1> point_ids;

  point_ids[0] = points->InsertNextPoint(1.0, 0.0, 0.0);
  vertices->InsertNextCell(1, point_ids.data());

  point_ids[0] = points->InsertNextPoint(0.0, 0.0, 0.0);
  vertices->InsertNextCell(1, point_ids.data());

  point_ids[0] = points->InsertNextPoint(0.0, 1.0, 0.0);
  vertices->InsertNextCell(1, point_ids.data());

  // create a polydata
  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points);
  polydata->SetVerts(vertices);

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputData(polydata);

  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(5);
  actor->GetProperty()->SetColor(colors->GetColor3d("Black").GetData());

  vtkNew<vtkRenderer> render;
  render->SetBackground(colors->GetColor3d("Gold").GetData());
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> render_window;
  render_window->AddRenderer(render);
  render_window->Render();

  vtkNew<vtkAreaPicker> picker;
  vtkNew<vtkCallbackCommand> pick_callback;
  pick_callback->SetCallback(pick_callback_function);
  picker->AddObserver(vtkCommand::EndPickEvent, pick_callback);

  vtkNew<vtkInteractorStyleRubberBandPick> rubber_band_style;
  rubber_band_style->SetCurrentRenderer(render);

  vtkNew<vtkInteractorStyleTrackballCamera> trackball_style;
  trackball_style->SetCurrentRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(render_window);
  interactor->SetPicker(picker);
  // interactor->SetInteractorStyle(rubber_band_style);
  interactor->SetInteractorStyle(trackball_style);
  interactor->Initialize();
  interactor->Start();
}