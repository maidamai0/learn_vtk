#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkNew.h"
#include "vtkPlot.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTable.h"

auto main(int argc, char** argv) -> int {
  constexpr int num = 1000;
  vtkNew<vtkTable> table;

  vtkNew<vtkIntArray> x_axis;
  x_axis->SetName("X axis");
  table->AddColumn(x_axis);

  vtkNew<vtkDoubleArray> y_axis;
  y_axis->SetName("Y axis");
  table->AddColumn(y_axis);

  table->SetNumberOfRows(num);
  vtkMath::RandomSeed(time(nullptr));
  for (auto i = 0; i < num; ++i) {
    table->SetValue(i, 0, i);
    table->SetValue(i, 1, vtkMath::Random(0.0, 2.0));
  }

  vtkNew<vtkChartXY> chart;
  auto line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 1);
  line->SetColor(0, 0, 0);

  vtkNew<vtkContextView> view;
  view->GetScene()->AddItem(chart);
  view->GetRenderer()->SetBackground(1, 1, 1);
  view->GetRenderWindow()->Render();
  view->GetInteractor()->Start();
}