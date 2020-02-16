#pragma once

/**
 * @file event_qt_slot_connect.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-02-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ui_event_qt_slot_connect.h"

#include <QMainWindow>

#include "vtkEventQtSlotConnect.h"
#include "vtkObject.h"
#include "vtkSmartPointer.h"

class EventQtSlotConnect : public QMainWindow, private Ui::EventQtSlotConnect {
  Q_OBJECT
public:
  EventQtSlotConnect();
public slots:
  void slot_clicked(vtkObject*, unsigned long, void*, void*);

private:
  vtkSmartPointer<vtkEventQtSlotConnect> connections_;
};