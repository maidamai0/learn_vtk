#include <QApplication>
#include <QQmlApplicationEngine>

#include <iostream>

auto main(int argc, char** argv) -> int {
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;
  engine.addImportPath("C:/Program Files (x86)/VTK/lib/qml");

  engine.load(QUrl("qrc:/main.qml"));

  if (engine.rootObjects().isEmpty()) {
    std::cout << "Load main.qml failed." << std::endl;
    return -1;
  }

  return QApplication::exec();
}