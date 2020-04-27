#include "fmt/core.h"

#include "vtkActor.h"
#include "vtkAreaPicker.h"
#include "vtkCallbackCommand.h"
#include "vtkCellArray.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkDataSetMapper.h"
#include "vtkExtractSelection.h"
#include "vtkIdTypeArray.h"
#include "vtkInteractorStyleRubberBandPick.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkNamedColors.h"
#include "vtkNew.h"
#include "vtkObject.h"
#include "vtkPlaneSource.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProp3DCollection.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkSelection.h"
#include "vtkSelectionNode.h"
#include "vtkSmartPointer.h"
#include "vtkTriangleFilter.h"
#include "vtkType.h"
#include "vtkUnstructuredGrid.h"

#include <array>
#include <iostream>

class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
  static auto New() -> MouseInteractorStyle*;

  MouseInteractorStyle()
    : selected_mapper_{vtkSmartPointer<vtkDataSetMapper>::New()},
      selected_actor_(vtkSmartPointer<vtkActor>::New()) {
  }

  void OnLeftButtonDown() override {
    vtkNew<vtkNamedColors> colors;
    auto point = GetInteractor()->GetEventPosition();
    vtkNew<vtkCellPicker> picker;
    picker->SetTolerance(0.0005);
    picker->Pick(point[0], point[1], 0, GetDefaultRenderer());
    fmt::print("Cell id is {}\n", picker->GetCellId());

    if (picker->GetCellId() != -1) {
      auto world_position = picker->GetPickPosition();
      fmt::print("Pick position is:[{},{},{}]\n", world_position[0],
                 world_position[1], world_position[2]);
      vtkNew<vtkIdTypeArray> ids;
      ids->SetNumberOfComponents(1);
      ids->InsertNextValue(picker->GetCellId());

      vtkNew<vtkSelectionNode> selection_node;
      selection_node->SetFieldType(vtkSelectionNode::CELL);
      selection_node->SetContentType(vtkSelectionNode::INDICES);
      selection_node->SetSelectionList(ids);

      vtkNew<vtkSelection> selection;
      selection->AddNode(selection_node);

      vtkNew<vtkExtractSelection> extract_selection;
      extract_selection->SetInputData(0, data_);
      extract_selection->SetInputData(1, selection);
      extract_selection->Update();

      vtkNew<vtkUnstructuredGrid> selected;
      selected->ShallowCopy(extract_selection->GetOutput());
      fmt::print("There are {} points and {} cells in the selection\n",
                 selected->GetNumberOfPoints(), selected->GetNumberOfCells());

      selected_mapper_->SetInputData(selected);
      selected_actor_->SetMapper(selected_mapper_);
      selected_actor_->GetProperty()->EdgeVisibilityOn();
      selected_actor_->GetProperty()->SetColor(
        colors->GetColor3d("Red").GetData());
      selected_actor_->GetProperty()->SetLineWidth(3);

      GetInteractor()
        ->GetRenderWindow()
        ->GetRenderers()
        ->GetFirstRenderer()
        ->AddActor(selected_actor_);
    }

    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }

  vtkSmartPointer<vtkPolyData> data_;
  vtkSmartPointer<vtkDataSetMapper> selected_mapper_;
  vtkSmartPointer<vtkActor> selected_actor_;
};

auto MouseInteractorStyle::New() -> MouseInteractorStyle* {
  auto res = new MouseInteractorStyle();
  res->InitializeObjectBase();
  return res;
}

auto main(int argc, char** argv) -> int {
  vtkNew<vtkPlaneSource> plane_source;
  plane_source->Update();

  vtkNew<vtkTriangleFilter> triangle_filter;
  triangle_filter->SetInputConnection(plane_source->GetOutputPort());
  triangle_filter->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(triangle_filter->GetOutputPort());

  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Green").GetData());

  vtkNew<vtkRenderer> render;
  render->SetBackground(colors->GetColor3d("Blue").GetData());
  render->AddActor(actor);

  vtkNew<vtkRenderWindow> render_window;
  render_window->AddRenderer(render);
  render_window->Render();

  vtkNew<MouseInteractorStyle> mouse_style;
  mouse_style->SetDefaultRenderer(render);
  mouse_style->data_ = triangle_filter->GetOutput();

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(render_window);
  interactor->SetInteractorStyle(mouse_style);
  interactor->Initialize();
  interactor->Start();
}