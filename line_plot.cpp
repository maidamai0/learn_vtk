#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkFloatArray.h"
#include "vtkNew.h"
#include "vtkPen.h"
#include "vtkPlot.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTable.h"

#include <ctime>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkTable> table;
  vtkNew<vtkFloatArray> x;
  x->SetName("X Axis");
  table->AddColumn(x);

  vtkNew<vtkFloatArray> cosine;
  cosine->SetName("Cosine");
  table->AddColumn(cosine);

  vtkNew<vtkFloatArray> sine;
  sine->SetName("Sine");
  table->AddColumn(sine);

  const std::size_t num = 69;
  const auto inc = 10.0 / (num - 1);
  table->SetNumberOfRows(num);

  for (auto i = 0; i < num; ++i) {
    const auto x_value = i * inc;
    table->SetValue(i, 0, x_value);
    table->SetValue(i, 1, cos(x_value));
    table->SetValue(i, 2, sin(x_value));
  }

  vtkNew<vtkChartXY> chart;
  auto line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 1);
  line->SetColor(255, 0, 0);
  line->SetWidth(2);

  line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 2);
  line->SetColor(0, 0, 255);
  line->SetWidth(2);
  line->GetPen()->SetLineType(vtkPen::DOT_LINE);

  vtkNew<vtkContextView> view;
  view->GetRenderer()->SetBackground(255, 255, 255);
  view->GetScene()->AddItem(chart);
  view->GetRenderWindow()->Render();
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();
}