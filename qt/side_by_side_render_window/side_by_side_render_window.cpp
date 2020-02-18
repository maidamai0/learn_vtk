#include "side_by_side_render_window.h"

#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCubeSource.h"
#include "vtkElevationFilter.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkNamedColors.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkSphereSource.h"

SideBySideRenderWindow::SideBySideRenderWindow() {
  setupUi(this);

  vtkNew<vtkGenericOpenGLRenderWindow> window_left;
  vtkNew<vtkGenericOpenGLRenderWindow> window_right;

  qvtkWidgetLeft->SetRenderWindow(window_left);
  qvtkWidgetRight->SetRenderWindow(window_right);

  vtkNew<vtkSphereSource> sphere_source;
  sphere_source->SetPhiResolution(30);
  sphere_source->SetThetaResolution(30);
  sphere_source->Update();

  vtkNew<vtkElevationFilter> sphere_elev;
  sphere_elev->SetInputConnection(sphere_source->GetOutputPort());
  sphere_elev->SetLowPoint(0, -1.0, 0);
  sphere_elev->SetHighPoint(0, 1.0, 0);

  vtkNew<vtkPolyDataMapper> sphere_mapper;
  sphere_mapper->SetInputConnection(sphere_elev->GetOutputPort());
  vtkNew<vtkActor> sphere_actor;
  sphere_actor->SetMapper(sphere_mapper);

  vtkNew<vtkCubeSource> cube_source;
  cube_source->Update();

  vtkNew<vtkElevationFilter> cube_elev;
  cube_elev->SetInputConnection(cube_elev->GetOutputPort());
  cube_elev->SetLowPoint(0, -1.0, 0);
  cube_elev->SetHighPoint(0, 1.0, 0);

  vtkNew<vtkPolyDataMapper> cube_mapper;
  cube_mapper->SetInputConnection(cube_source->GetOutputPort());
  vtkNew<vtkActor> cube_actor;
  cube_actor->SetMapper(cube_mapper);

  vtkNew<vtkRenderer> render_left;
  render_left->AddActor(sphere_actor);

  vtkNew<vtkRenderer> render_right;
  render_right->AddActor(cube_actor);
  render_right->GetActiveCamera()->SetPosition(1.0, 0.8, 1.0);
  render_right->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  render_right->ResetCamera();
  render_right->GetActiveCamera()->Zoom(0.8);

  vtkNew<vtkNamedColors> colors;
  render_left->SetBackground(colors->GetColor4d("LightSteelBlue").GetData());
  render_right->SetBackground(colors->GetColor4d("LightSteelBlue").GetData());

  qvtkWidgetLeft->GetRenderWindow()->AddRenderer(render_left);
  qvtkWidgetRight->GetRenderWindow()->AddRenderer(render_right);
}

void SideBySideRenderWindow::slot_exit() {
  qApp->exit();
}