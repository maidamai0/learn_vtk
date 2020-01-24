#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkDataObject.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkMapper.h"
#include "vtkMath.h"
#include "vtkObject.h"
#include "vtkPolyDataMapper.h"
#include "vtkProgrammableAttributeDataFilter.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkScalarBarActor.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"

#include <cfloat>

namespace {
vtkSmartPointer<vtkProgrammableAttributeDataFilter> random_colors;
};

void color_cells(void* arg1) {
  const auto random_number_generator = vtkSmartPointer<vtkMath>::New();
  const auto input = random_colors->GetInput();
  auto outpt = random_colors->GetOutput();

  const auto num_cells = input->GetNumberOfElements(vtkDataObject::CELL);

  const auto colors = vtkSmartPointer<vtkFloatArray>::New();
  colors->SetNumberOfTuples(num_cells);

  int i = 0;
  while (i < num_cells) {
    colors->SetValue(i, random_number_generator->Random(0, 1));
    ++i;
  }

  outpt->GetCellData()->CopyScalarsOff();
  outpt->GetCellData()->SetScalars(colors);
}

int main() {
  const auto sphere_source = vtkSmartPointer<vtkSphereSource>::New();
  sphere_source->SetThetaResolution(20);
  sphere_source->SetPhiResolution(40);

  random_colors = vtkSmartPointer<vtkProgrammableAttributeDataFilter>::New();
  random_colors->SetInputConnection(sphere_source->GetOutputPort());
  random_colors->SetExecuteMethod(color_cells, nullptr);

  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(random_colors->GetOutputPort());
  mapper->SetScalarRange(random_colors->GetPolyDataOutput()->GetScalarRange());

  const auto sphere_actor = vtkSmartPointer<vtkActor>::New();
  sphere_actor->SetMapper(mapper);

  const auto scalar_bar = vtkSmartPointer<vtkScalarBarActor>::New();
  scalar_bar->SetLookupTable(mapper->GetLookupTable());
  scalar_bar->SetTitle("Temperature");
  scalar_bar->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedViewport();
  scalar_bar->GetPositionCoordinate()->SetValue(0.1, 0.01);
  scalar_bar->SetOrientationToHorizontal();
  scalar_bar->SetWidth(0.8);
  scalar_bar->SetHeight(0.17);
  scalar_bar->SetPosition(scalar_bar->GetPosition());

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(sphere_actor);
  render->AddActor2D(scalar_bar);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->SetSize(350, 350);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render->ResetCamera();
  render->GetActiveCamera()->Zoom(1.5);

  scalar_bar->SetNumberOfLabels(8);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}