#include "vtkTesting.h"
#include <ios>
#include <iostream>
#include <string>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCoordinate.h>
#include <vtkLineSource.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRegressionTestImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTestUtilities.h>

#include "common.h"

auto main(int argc, char* argv[]) -> int {
  vtkNew<vtkLookupTable> lut;
  lut->SetNumberOfTableValues(6);
  lut->SetTableRange(0.0, 1.0);

  vtkNew<vtkPlaneSource> planeSource1;
  planeSource1->SetOrigin(0.0, 0.0, 0.0);
  planeSource1->SetPoint1(0.5, 0.0, 0.0);
  planeSource1->SetPoint2(0.0, 0.5, 0.0);

  vtkNew<vtkPolyDataMapper> mapper1;
  mapper1->SetInputConnection(planeSource1->GetOutputPort());
  mapper1->ScalarVisibilityOn();
  mapper1->SetLookupTable(lut);
  mapper1->UseLookupTableScalarRangeOn();
  mapper1->SetScalarModeToUsePointFieldData();
  mapper1->ColorByArrayComponent("TextureCoordinates", 0);
  mapper1->InterpolateScalarsBeforeMappingOn();

  vtkNew<vtkActor> actor1;
  actor1->SetMapper(mapper1);
  actor1->GetProperty()->SetColor(1.0, 0.0, 0.0);

  vtkNew<vtkPlaneSource> planeSource2;
  planeSource2->SetOrigin(-0.5, 0.0, 0.0);
  planeSource2->SetPoint1(0.0, 0.0, 0.0);
  planeSource2->SetPoint2(-0.5, 0.5, 0.0);

  vtkNew<vtkCoordinate> pCoord;
  pCoord->SetCoordinateSystemToWorld();

  vtkNew<vtkCoordinate> coord;
  coord->SetCoordinateSystemToNormalizedViewport();
  coord->SetReferenceCoordinate(pCoord);

  vtkNew<vtkPolyDataMapper2D> mapper2;
  mapper2->SetInputConnection(planeSource2->GetOutputPort());
  mapper2->SetLookupTable(lut);
  mapper2->ScalarVisibilityOff();
  mapper2->SetTransformCoordinate(coord);

  vtkNew<vtkActor2D> actor2;
  actor2->SetMapper(mapper2);
  actor2->GetProperty()->SetColor(1.0, 1.0, 0.0);

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor1);
  renderer->AddActor(actor2);

  vtkNew<vtkRenderWindow> renWin;
  renWin->AddRenderer(renderer);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin);
  renWin->Render();
  renderer->ResetCamera();
  renderer->SetBackground(1.0, 0.0, 0.0);
  renWin->SetSize(300, 300);

  vtkNew<vtkTesting> test;
  test->AddArgument("-D");
  test->AddArgument(base_img_dir.c_str());
  test->AddArgument("-V");
  test->AddArgument("test_actor_2d.png");
  test->AddArgument("-T");
  test->AddArgument(temp_img_dir.c_str());
  test->SetRenderWindow(renWin);
  const auto passed = (test->RegressionTest(0.15) == vtkTesting::PASSED);
  std::boolalpha(std::cout);
  std::cout << "passed?" << passed << std::endl;
}
