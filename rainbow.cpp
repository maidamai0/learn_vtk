
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkDataArray.h"
#include "vtkDataObject.h"
#include "vtkLookupTable.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkStructuredGridOutlineFilter.h"

#include "platform.h"

int main() {
  const auto reader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
  reader->SetXYZFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  reader->SetQFileName((fs::canonical("Data").string() + "/combq.bin").c_str());
  reader->SetScalarFunctionNumber(100);
  reader->SetVectorFunctionNumber(202);
  reader->Update();

  const auto reader_out = reader->GetOutput()->GetBlock(0);

  const auto plane = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
  plane->SetInputData(reader_out);
  plane->SetExtent(1, 100, 1, 100, 7, 7);

  const auto lut = vtkSmartPointer<vtkLookupTable>::New();

  // black to white
  //   lut->SetHueRange(0, 0);
  //   lut->SetSaturationRange(0, 0);
  //   lut->SetValueRange(0.2, 1.0);

  // red and blue
  lut->SetHueRange(0.0, 0.667);

  // blue to red
  //   lut->SetHueRange(0.667, 0.0);

  // weird effect
  //   lut->SetNumberOfColors(256);
  //   lut->Build();
  //   for (int i = 0; i < 16; ++i) {
  //     lut->SetTableValue(i * 16, 1.0, 0, 0, 1);
  //     lut->SetTableValue(i * 16 + 1, 0, 1.0, 0.0, 1);
  //     lut->SetTableValue(i * 16 + 2, 0, 0, 1.0, 1);
  //     lut->SetTableValue(i * 16 + 3, 0, 0, 0, 1);
  //   }

  const auto plane_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  plane_mapper->SetLookupTable(lut);
  plane_mapper->SetInputConnection(plane->GetOutputPort());
  plane_mapper->SetScalarRange(0.197813, 0.710419);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(plane_mapper);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputData(reader_out);

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(outline_actor);
  render->SetBackground(0.1, 0.2, 0.4);
  render->TwoSidedLightingOff();

  const auto camera = render->GetActiveCamera();
  camera->SetClippingRange(3.95297, 50);
  camera->SetFocalPoint(8.88908, 0.595038, 29.3342);
  camera->SetPosition(-12.3332, 31.7479, 41.2387);
  camera->SetViewUp(0.060772, -0.319905, 0.945498);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  render_window->Render();
  interactor->Initialize();
  interactor->Start();
}