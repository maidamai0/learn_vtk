#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

int main() {
  const auto points = vtkSmartPointer<vtkPoints>::New();
  points->InsertPoint(0, 0.0, 0.0, 1.0);
  points->InsertPoint(1, 0.0, 1.0, 0.0);
  points->InsertPoint(2, 1.0, 0.0, 0.0);
  points->InsertPoint(3, 1.0, 1.0, 0.0);
  points->InsertPoint(4, 2.0, 0.0, 5.0);
  points->InsertPoint(5, 2.0, 1.0, 0.0);
  points->InsertPoint(6, 3.0, 0.0, 0.0);
  points->InsertPoint(7, 3.0, 1.0, 0.0);

  const auto strips = vtkSmartPointer<vtkCellArray>::New();
  strips->InsertNextCell(points->GetNumberOfPoints());
  for (int i = 0; i < points->GetNumberOfPoints(); ++i) {
    strips->InsertCellPoint(i);
  }

  const auto profile = vtkSmartPointer<vtkPolyData>::New();
  profile->SetPoints(points);
  profile->SetStrips(strips);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(profile);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0.1, 0.2, 0.4);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);

  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer(render);
  window->Render();

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(window);
  interactor->Initialize();
  interactor->Start();
}