#include "show_event.h"
#include "qwidget.h"
#include <iostream>

ShowEvent::ShowEvent(QWidget* parent) : QWidget(parent) {
  setupUi(this);
}

void ShowEvent::showEvent(QShowEvent* evt) {
}