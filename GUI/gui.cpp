#include "gui.h"

GUI::GUI(QObject *parent, GuiType type) : QObject(parent) {
  Type = type;

  MainWidget = new QWidget();
  MainLayout = new QHBoxLayout();
  MainWidget->setLayout(MainLayout);
}

GUI::~GUI() { delete MainWidget; }

GUI::GuiType GUI::type() { return Type; }
