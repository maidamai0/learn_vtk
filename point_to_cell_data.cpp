#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkConnectivityFilter.h"
#include "vtkContourFilter.h"
#include "vtkDataArray.h"
#include "vtkDataSetMapper.h"
#include "vtkGeometryFilter.h"
#include "vtkLookupTable.h"
#include "vtkPointDataToCellData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkThreshold.h"
#include "vtkThresholdPoints.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkWarpVector.h"

#include "platform.h"
#include <filesystem>
#include <memory>

int main() {
  const auto reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
  reader->SetFileName((fs::canonical("Data").string() + "/blow.vtk").c_str());
  reader->SetScalarsName("thickness9");
  reader->SetVectorsName("displacement9");
  //   reader->Update();

  const auto convert = vtkSmartPointer<vtkPointDataToCellData>::New();
  convert->SetInputConnection(reader->GetOutputPort());
  convert->PassPointDataOn();

  const auto warp = vtkSmartPointer<vtkWarpVector>::New();
  warp->SetInputConnection(convert->GetOutputPort());

  const auto thresh = vtkSmartPointer<vtkThreshold>::New();
  thresh->SetInputConnection(warp->GetOutputPort());
  thresh->ThresholdBetween(0.25, 0.75);
  thresh->SetInputArrayToProcess(1, 0, 0, 0, "thickness9");
  thresh->Update();

  const auto connect = vtkSmartPointer<vtkConnectivityFilter>::New();
  connect->SetInputConnection(thresh->GetOutputPort());
  connect->SetExtractionModeToSpecifiedRegions();
  connect->AddSpecifiedRegion(0);
  connect->AddSpecifiedRegion(1);

  const auto mapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(connect->GetOutputPort());
  mapper->ScalarVisibilityOff();

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0.2, 0.2, 0.2);
  actor->GetProperty()->SetRepresentationToWireframe();

  const auto connet_1 = vtkSmartPointer<vtkConnectivityFilter>::New();
  connet_1->SetInputConnection(thresh->GetOutputPort());

  const auto geometry = vtkSmartPointer<vtkGeometryFilter>::New();
  geometry->SetInputConnection(connet_1->GetOutputPort());

  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(geometry->GetOutputPort());
  normals->SetFeatureAngle(60);

  const auto lookup_table = vtkSmartPointer<vtkLookupTable>::New();
  lookup_table->SetHueRange(0.0, 0.66667);

  const auto mapper_1 = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper_1->SetInputConnection(normals->GetOutputPort());
  mapper_1->SetLookupTable(lookup_table);
  mapper_1->SetScalarRange(0.12, 1.0);

  const auto actor_1 = vtkSmartPointer<vtkActor>::New();
  actor_1->SetMapper(mapper_1);

  const auto cf = vtkSmartPointer<vtkContourFilter>::New();
  cf->SetInputConnection(connet_1->GetOutputPort());
  cf->SetValue(0, 0.5);

  const auto cf_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  cf_mapper->SetInputConnection(cf->GetOutputPort());

  const auto cf_actor = vtkSmartPointer<vtkActor>::New();
  cf_actor->SetMapper(cf_mapper);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->AddActor(actor);
  render->AddActor(actor_1);
  render->AddActor(cf_actor);
  render->ResetCameraClippingRange();
  render->SetBackground(1, 1, 1);
  render->ResetCamera();
  const auto camera = render->GetActiveCamera();
  camera->Azimuth(60);
  camera->Roll(-90);
  camera->Dolly(2);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}