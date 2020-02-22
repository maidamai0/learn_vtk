#include "vtkActor2D.h"
#include "vtkCoordinate.h"
#include "vtkNamedColors.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"

#include "platform.h"

auto main(int argc, char** argv) -> int {
  const int font_size = 24;
  const double line_spaceing = 0.8;

  const auto single_line_prop = vtkSmartPointer<vtkTextProperty>::New();
  single_line_prop->SetFontSize(font_size);
  single_line_prop->SetFontFamilyToArial();
  single_line_prop->SetBold(VTK_FALSE);
  single_line_prop->SetItalic(VTK_FALSE);
  single_line_prop->SetShadow(VTK_FALSE);

  const auto multiline_line_prop = vtkSmartPointer<vtkTextProperty>::New();
  multiline_line_prop->ShallowCopy(single_line_prop);
  multiline_line_prop->SetBold(VTK_TRUE);
  multiline_line_prop->SetItalic(VTK_TRUE);
  multiline_line_prop->SetShadow(VTK_TRUE);
  multiline_line_prop->SetLineSpacing(line_spaceing);

  const auto colors = vtkSmartPointer<vtkNamedColors>::New();

  const auto single_line_text_mapper_bottom
    = vtkSmartPointer<vtkTextMapper>::New();
  single_line_text_mapper_bottom->SetInput("Single Line(bottom)");

  auto text_property = single_line_text_mapper_bottom->GetTextProperty();
  text_property->ShallowCopy(single_line_prop);
  text_property->SetVerticalJustificationToBottom();
  text_property->SetColor(colors->GetColor3d("Tomata").GetData());

  const auto single_line_text_actor_bottom = vtkSmartPointer<vtkActor2D>::New();
  single_line_text_actor_bottom->SetMapper(single_line_text_mapper_bottom);
  single_line_text_actor_bottom->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedDisplay();
  single_line_text_actor_bottom->GetPositionCoordinate()->SetValue(0.05, 0.85);

  const auto single_line_text_mapper_center
    = vtkSmartPointer<vtkTextMapper>::New();
  single_line_text_mapper_center->SetInput("Single line(centerd)");
  text_property = single_line_text_mapper_center->GetTextProperty();
  text_property->ShallowCopy(single_line_prop);
  text_property->SetVerticalJustificationToCentered();
  text_property->SetColor(colors->GetColor4d("DarkerGreen").GetData());

  const auto single_line_text_actor_center = vtkSmartPointer<vtkActor2D>::New();
  single_line_text_actor_center->SetMapper(single_line_text_mapper_center);
  single_line_text_actor_center->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedDisplay();
  single_line_text_actor_center->GetPositionCoordinate()->SetValue(0.05, 0.75);

  const auto single_line_text_mapper_top
    = vtkSmartPointer<vtkTextMapper>::New();
  single_line_text_mapper_top->SetInput("Single line (Top)");
  text_property = single_line_text_mapper_top->GetTextProperty();
  text_property->ShallowCopy(single_line_prop);
  text_property->SetVerticalJustificationToTop();
  text_property->SetColor(colors->GetColor4d("Peacock").GetData());

  const auto single_line_text_actor_top = vtkSmartPointer<vtkActor2D>::New();
  single_line_text_actor_top->SetMapper(single_line_text_mapper_top);
  single_line_text_actor_top->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedDisplay();
  single_line_text_actor_top->GetPositionCoordinate()->SetValue(0.05, 0.65);

  const auto multi_line_mapper_left = vtkSmartPointer<vtkTextMapper>::New();
  multi_line_mapper_left->SetInput(
    "This is \nMulti_line\ntext output\n(left-top)");
  text_property = multi_line_mapper_left->GetTextProperty();
  text_property->ShallowCopy(multiline_line_prop);
  text_property->SetJustificationToLeft();
  text_property->SetVerticalJustificationToTop();
  text_property->SetColor(colors->GetColor4d("Tomata").GetData());

  const auto multi_line_actor_left = vtkSmartPointer<vtkActor2D>::New();
  multi_line_actor_left->SetMapper(multi_line_mapper_left);
  multi_line_actor_left->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedDisplay();
  multi_line_actor_left->GetPositionCoordinate()->SetValue(0.05, 0.5);

  const auto multi_line_mapper_center = vtkSmartPointer<vtkTextMapper>::New();
  multi_line_mapper_center->SetInput(
    "This is \nMulti line\ntext output(centerd)");
  text_property = multi_line_mapper_center->GetTextProperty();
  text_property->ShallowCopy(multiline_line_prop);
  text_property->SetJustificationToCentered();
  text_property->SetVerticalJustificationToCentered();
  text_property->SetColor(colors->GetColor4d("DarkGreen").GetData());

  const auto multi_line_actor_center = vtkSmartPointer<vtkActor2D>::New();
  multi_line_actor_center->SetMapper(multi_line_mapper_center);
  multi_line_actor_center->GetPositionCoordinate()
    ->SetCoordinateSystemToNormalizedDisplay();
  multi_line_actor_center->GetPositionCoordinate()->SetValue(0.5, 0.5);

  vtkNew<vtkRenderer> render;
  render->AddActor(single_line_text_actor_bottom);
  render->AddActor(single_line_text_actor_center);
  render->AddActor(single_line_text_actor_top);
  render->AddActor(multi_line_actor_center);
  render->AddActor(multi_line_actor_left);

  vtkNew<vtkRenderWindow> window;
  window->AddRenderer(render);

  vtkNew<vtkRenderWindowInteractor> interactor;
  interactor->SetRenderWindow(window);
  window->Render();
  interactor->Initialize();
  interactor->Start();
}