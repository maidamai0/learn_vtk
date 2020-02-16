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

#include "ui_render_window_ui_file_single_inheritance.h"

#include <QMainWindow>

class RenderWindowUIFIleSingleleInheritance : public QMainWindow {
  Q_OBJECT
public:
  RenderWindowUIFIleSingleleInheritance();
  ~RenderWindowUIFIleSingleleInheritance() = default;
public slots:
  void slot_exit();

private:
  Ui_RenderWindowUISingleInheritance* ui_;
};