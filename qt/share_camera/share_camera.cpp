#include "share_camera.h"

#include "qapplication.h"
#include "qobjectdefs.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

ShareCamera::ShareCamera() {
  setupUi(this);
  vtkNew<vtkGenericOpenGLRenderWindow> window_left;
  vtkNew<vtkGenericOpenGLRenderWindow> window_right;

  qvtkWidgetLeft->setRenderWindow(window_left);
  qvtkWidgetRight->setRenderWindow(window_right);

  vtkNew<vtkSphereSource> source;
  source->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(source->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkCubeSource> cube_source;
  vtkNew<vtkPolyDataMapper> cube_mapper;
  cube_mapper->SetInputConnection(cube_source->GetOutputPort());

  vtkNew<vtkActor> cube_actor;
  cube_actor->SetMapper(cube_mapper);

  vtkNew<vtkRenderer> render_left;
  render_left->AddActor(actor);

  vtkNew<vtkRenderer> render_right;
  render_right->AddActor(cube_actor);

  vtkNew<vtkNamedColors> colors;
  actor->GetProperty()->SetColor(colors->GetColor4d("Tomato").GetData());
  cube_actor->GetProperty()->SetColor(
    colors->GetColor4d("MediumSeaGreen").GetData());
  render_left->SetBackground(colors->GetColor4d("LightSteelBlue").GetData());
  render_right->SetBackground(colors->GetColor4d("LightSteelBlue").GetData());

  qvtkWidgetLeft->renderWindow()->AddRenderer(render_left);
  qvtkWidgetRight->renderWindow()->AddRenderer(render_right);

  render_left->ResetCamera();
  render_right->ResetCamera();

  render_right->SetActiveCamera(render_left->GetActiveCamera());

  render_left->GetActiveCamera()->SetPosition(1.0, 0.8, 1.0);
  render_left->GetActiveCamera()->SetFocalPoint(0.0, 0.0, 0.0);
  render_left->ResetCamera();
  render_left->GetActiveCamera()->Zoom(0.8);

  connect(this->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

  qvtkWidgetLeft->renderWindow()->AddObserver(
    vtkCommand::ModifiedEvent, this, &ShareCamera::left_modified_handler);

  // qvtkWidgetRight->renderWindow()->AddObserver(
  //   vtkCommand::ModifiedEvent, this, &ShareCamera::right_modified_handler);
}

void ShareCamera::left_modified_handler() {
  qvtkWidgetRight->renderWindow()->Render();
}

void ShareCamera::right_modified_handler() {
  qvtkWidgetLeft->renderWindow()->Render();
}

void ShareCamera::slotExit() {
  qApp->exit();
}