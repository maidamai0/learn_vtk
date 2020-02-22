#include <array>
#include <ctime>

#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "vtkNew.h"
#include "vtkPlot.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTable.h"

auto main(int argc, char** argv) -> int {
  vtkMath::RandomSeed(time(nullptr));

  vtkNew<vtkTable> table;
  vtkNew<vtkIntArray> x;
  x->SetName("X Axis");
  table->AddColumn(x);

  vtkNew<vtkFloatArray> y;
  y->SetName("Y axis");
  table->AddColumn(y);

  const auto num = 1000;
  table->SetNumberOfRows(num);
  for (auto i = 0; i < num; ++i) {
    table->SetValue(i, 0, i);
    table->SetValue(i, 1, vtkMath::Gaussian(0.0, 2.0));
  }

  vtkNew<vtkChartXY> chart;
  auto line = chart->AddPlot(vtkChart::LINE);
  line->SetWidth(2);
  line->SetInputData(table, 0, 1);

  vtkNew<vtkContextView> view;
  view->GetScene()->AddItem(chart);
  view->GetRenderer()->SetBackground(255, 255, 255);
  view->GetRenderWindow()->Render();
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();
}