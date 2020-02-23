#include "fmt/core.h"

#include "vtkChartXY.h"
#include "vtkContextScene.h"
#include "vtkContextView.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkMinimalStandardRandomSequence.h"
#include "vtkNew.h"
#include "vtkPlot.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkTable.h"

#include <array>
#include <cstdlib>

auto main(int argc, char** argv) -> int {
  vtkNew<vtkMinimalStandardRandomSequence> sequence;
  sequence->SetSeed(1);

  constexpr std::size_t size{20};

  vtkNew<vtkIntArray> x_axis;
  x_axis->SetName("X axis");
  vtkNew<vtkDoubleArray> y_axis;
  y_axis->SetName("Y axis");

  vtkNew<vtkTable> table;
  table->AddColumn(x_axis);
  table->AddColumn(y_axis);
  table->SetNumberOfRows(size);

  fmt::print("Random values are:\n");
  for (auto i = 0; i < size; ++i) {
    table->SetValue(i, 0, i);
    table->SetValue(i, 1, sequence->GetValue());
    fmt::print("{}->{}\n", i, sequence->GetValue());

    sequence->Next();
  }

  vtkNew<vtkChartXY> chart;
  auto line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 1);
  line->SetColor(0.1, 0.2, 0.4);

  vtkNew<vtkContextView> view;
  view->GetScene()->AddItem(chart);

  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  view->GetRenderWindow()->Render();
  view->GetInteractor()->Initialize();
  view->GetInteractor()->Start();
}