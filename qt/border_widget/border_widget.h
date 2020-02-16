#pragma once
/**
 * @file border_widget.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-02-15
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <QMainWindow>

#include "ui_border_widget.h"

#include "vtkBorderWidget.h"
#include "vtkSmartPointer.h"

class BorderWidget final : public QMainWindow, private Ui::BorderWidgetQt {
  Q_OBJECT;

public:
  BorderWidget();
  ~BorderWidget() final = default;

private:
  vtkSmartPointer<vtkBorderWidget> border_widget_;
};