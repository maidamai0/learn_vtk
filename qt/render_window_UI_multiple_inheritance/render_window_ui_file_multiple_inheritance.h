#pragma once

/**
 * @file render_window_ui_file_multiple_inheritance.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ui_render_window_ui_file_multiple_inheritance.h"

#include <QMainWindow>

class RenderWindowUIFIleMultipleInheritance
  : public QMainWindow,
    private Ui::RenderWindowUIMultipleInheritance {
  Q_OBJECT
public:
  RenderWindowUIFIleMultipleInheritance();
public slots:
  void slot_exit();
};