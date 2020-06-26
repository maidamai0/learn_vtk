#pragma once

/**
 * @file share_camera.h
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
#include "ui_share_camera.h"

class ShareCamera final: public QMainWindow, private Ui_ShareCameraQt {
  Q_OBJECT
public:
  ShareCamera();
  ~ShareCamera() final= default;
public slots:
  void slotExit();

private:
  void left_modified_handler();
  void right_modified_handler();
};