#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkCamera.h"
#include "vtkLineSource.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkMultiBlockPLOT3DReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkProp3D.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGridOutlineFilter.h"
#include "vtkTextProperty.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTubeFilter.h"
#include "vtkXYPlotActor.h"

#include "platform.h"
#include <filesystem>

int main() {
  const auto plot_3d = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
  plot_3d->SetXYZFileName(
    (fs::canonical("Data").string() + "/combxyz.bin").c_str());
  plot_3d->SetQFileName(
    (fs::canonical("Data").string() + "/combq.bin").c_str());
  plot_3d->SetScalarFunctionNumber(100);
  plot_3d->SetVectorFunctionNumber(202);
  plot_3d->Update();
  const auto plot_3d_output = plot_3d->GetOutput()->GetBlock(0);

  const auto line_source = vtkSmartPointer<vtkLineSource>::New();
  line_source->SetResolution(30);

  const auto transform = vtkSmartPointer<vtkTransform>::New();
  transform->Translate(3.7, 0.0, 28.37);
  transform->Scale(5, 5, 5);
  transform->RotateY(90);

  const auto trans_filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  trans_filter->SetInputConnection(line_source->GetOutputPort());
  trans_filter->SetTransform(transform);

  const auto probe_filter = vtkSmartPointer<vtkProbeFilter>::New();
  probe_filter->SetInputConnection(trans_filter->GetOutputPort());
  probe_filter->SetSourceData(plot_3d_output);

  const auto transform_1 = vtkSmartPointer<vtkTransform>::New();
  transform_1->Translate(9.2, 0.0, 31.2);
  transform_1->Scale(5, 5, 5);
  transform_1->RotateY(90);

  const auto trans_filter_1
    = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  trans_filter_1->SetInputConnection(line_source->GetOutputPort());
  trans_filter_1->SetTransform(transform_1);

  const auto probe_filter_1 = vtkSmartPointer<vtkProbeFilter>::New();
  probe_filter_1->SetInputConnection(trans_filter_1->GetOutputPort());
  probe_filter_1->SetSourceData(plot_3d_output);

  const auto transform_2 = vtkSmartPointer<vtkTransform>::New();
  transform_2->Translate(13.27, 0.0, 33.4);
  transform_2->Scale(4.5, 4.5, 4.5);
  transform_2->RotateY(90);

  const auto trans_filter_2
    = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  trans_filter_2->SetInputConnection(line_source->GetOutputPort());
  trans_filter_2->SetTransform(transform_2);

  const auto probe_filter_2 = vtkSmartPointer<vtkProbeFilter>::New();
  probe_filter_2->SetInputConnection(trans_filter_2->GetOutputPort());
  probe_filter_2->SetSourceData(plot_3d_output);

  const auto append_polydata = vtkSmartPointer<vtkAppendPolyData>::New();
  append_polydata->AddInputConnection(probe_filter->GetOutputPort());
  append_polydata->AddInputConnection(probe_filter_1->GetOutputPort());
  append_polydata->AddInputConnection(probe_filter_2->GetOutputPort());

  const auto tuber = vtkSmartPointer<vtkTubeFilter>::New();
  tuber->SetInputConnection(append_polydata->GetOutputPort());
  tuber->SetRadius(0.1);

  const auto line_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  line_mapper->SetInputConnection(tuber->GetOutputPort());

  const auto line_actor = vtkSmartPointer<vtkActor>::New();
  line_actor->SetMapper(line_mapper);

  const auto xyplot_actor = vtkSmartPointer<vtkXYPlotActor>::New();
  xyplot_actor->AddDataSetInputConnection(probe_filter->GetOutputPort());
  xyplot_actor->AddDataSetInputConnection(probe_filter_1->GetOutputPort());
  xyplot_actor->AddDataSetInputConnection(probe_filter_2->GetOutputPort());
  xyplot_actor->GetPositionCoordinate()->SetValue(0.0, 0.67, 0.0);
  xyplot_actor->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0.0);
  xyplot_actor->SetXValuesToArcLength();
  xyplot_actor->SetNumberOfLabels(6);
  xyplot_actor->SetTitle("Pressure vs. Arc Length(Zoomed View)");
  xyplot_actor->SetXTitle("");
  xyplot_actor->SetYTitle("P");
  xyplot_actor->SetXRange(0.1, 0.3);
  xyplot_actor->SetYRange(0.2, 0.4);
  const auto xyplot_property = xyplot_actor->GetProperty();
  xyplot_property->SetColor(0, 0, 0);
  xyplot_property->SetLineWidth(2);

  auto xyplot_title_text_property = xyplot_actor->GetTitleTextProperty();
  xyplot_title_text_property->SetColor(xyplot_property->GetColor());
  xyplot_actor->SetAxisTitleTextProperty(xyplot_title_text_property);
  xyplot_actor->SetAxisLabelTextProperty(xyplot_title_text_property);

  const auto xyplot_actor_1 = vtkSmartPointer<vtkXYPlotActor>::New();
  xyplot_actor_1->AddDataSetInputConnection(probe_filter->GetOutputPort());
  xyplot_actor_1->AddDataSetInputConnection(probe_filter_1->GetOutputPort());
  xyplot_actor_1->AddDataSetInputConnection(probe_filter_2->GetOutputPort());
  xyplot_actor_1->GetPositionCoordinate()->SetValue(0.0, 0.33, 0);
  xyplot_actor_1->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0);
  xyplot_actor_1->SetXValuesToNormalizedArcLength();
  xyplot_actor_1->SetNumberOfLabels(6);
  xyplot_actor_1->SetTitle("Pressure vs. Normalized Arc Length");
  xyplot_actor_1->SetXTitle("");
  xyplot_actor_1->SetYTitle("");
  xyplot_actor_1->PlotPointsOn();
  xyplot_actor_1->PlotLinesOff();
  xyplot_actor_1->GetProperty()->SetColor(1, 0, 0);
  xyplot_actor_1->GetProperty()->SetPointSize(2);

  xyplot_title_text_property = xyplot_actor_1->GetTitleTextProperty();
  xyplot_title_text_property->SetColor(
    xyplot_actor_1->GetProperty()->GetColor());
  xyplot_actor_1->SetAxisTitleTextProperty(xyplot_title_text_property);
  xyplot_actor_1->SetAxisLabelTextProperty(xyplot_title_text_property);

  const auto xyplot_actor_2 = vtkSmartPointer<vtkXYPlotActor>::New();
  xyplot_actor_2->AddDataSetInputConnection(probe_filter->GetOutputPort());
  xyplot_actor_2->AddDataSetInputConnection(probe_filter_1->GetOutputPort());
  xyplot_actor_2->AddDataSetInputConnection(probe_filter_2->GetOutputPort());
  xyplot_actor_2->GetPositionCoordinate()->SetValue(0.0, 0.0, 0.0);
  xyplot_actor_2->GetPosition2Coordinate()->SetValue(1.0, 0.33, 0);
  xyplot_actor_2->SetXValuesToIndex();
  xyplot_actor_2->SetNumberOfLabels(6);
  xyplot_actor_2->SetTitle("Pressure vs. Point Id");
  xyplot_actor_2->SetXTitle("Probe Length");
  xyplot_actor_2->SetYTitle("Y");
  xyplot_actor_2->PlotPointsOn();
  xyplot_actor_2->GetProperty()->SetColor(0, 0, 1);
  xyplot_actor_2->GetProperty()->SetPointSize(2);

  xyplot_title_text_property = xyplot_actor_2->GetTitleTextProperty();
  xyplot_title_text_property->SetColor(
    xyplot_actor_2->GetProperty()->GetColor());
  xyplot_actor_2->SetAxisTitleTextProperty(xyplot_title_text_property);
  xyplot_actor_2->SetAxisLabelTextProperty(xyplot_title_text_property);

  const auto outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
  outline->SetInputData(plot_3d_output);

  const auto outline_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outline_mapper->SetInputConnection(outline->GetOutputPort());

  const auto outline_actor = vtkSmartPointer<vtkActor>::New();
  outline_actor->SetMapper(outline_mapper);
  outline_actor->GetProperty()->SetColor(0, 0, 0);

  const auto render = vtkSmartPointer<vtkRenderer>::New();
  render->SetBackground(0.6784, 0.8471, 0.9020);
  render->SetViewport(0, 0, 0.5, 1);
  render->AddActor(outline_actor);
  render->AddActor(line_actor);

  const auto render_1 = vtkSmartPointer<vtkRenderer>::New();
  render_1->SetBackground(1, 1, 1);
  render_1->SetViewport(0.5, 0.0, 1.0, 1);
  render_1->AddActor2D(xyplot_actor);
  render_1->AddActor2D(xyplot_actor_1);
  render_1->AddActor(xyplot_actor_2);

  const auto render_window = vtkSmartPointer<vtkRenderWindow>::New();
  render_window->AddRenderer(render);
  render_window->AddRenderer(render_1);
  render_window->SetSize(500, 250);

  const auto camera = render->GetActiveCamera();
  camera->SetClippingRange(3.95297, 100);
  camera->SetFocalPoint(8.88908, 0.595038, 29.3342);
  camera->SetPosition(-12.3332, 31.7479, 41.2387);
  camera->SetViewUp(0.060772, -0.319905, 0.945498);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(render_window);

  interactor->Initialize();
  render_window->Render();
  interactor->Start();
}