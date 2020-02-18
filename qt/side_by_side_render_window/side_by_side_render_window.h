#pragma once

/**
 * @file side_by_side_render_window.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-02-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "qmainwindow.h"
#include "qobjectdefs.h"
#include "ui_side_by_side_render_window.h"

class SideBySideRenderWindow : public QMainWindow,
                               Ui_SideBySideRenderWindowsQt {
  Q_OBJECT

public:
  SideBySideRenderWindow();
  ~SideBySideRenderWindow() = default;

public slots:
  void slot_exit();
};