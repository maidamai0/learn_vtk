#pragma once

/**
 * @file show_event.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-02-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "qobjectdefs.h"
#include "qwidget.h"
#include "ui_show_event.h"

class ShowEvent final : public QWidget, private Ui_ShowEvent {
  Q_OBJECT
public:
  ShowEvent(QWidget* parent = nullptr);

protected:
  void showEvent(QShowEvent* evt) override;
};