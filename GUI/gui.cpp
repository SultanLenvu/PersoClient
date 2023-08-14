#include "gui.h"

GUI::GUI(QWidget* parent, GuiType type) : QWidget(parent) {
  Type = type;

  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);
}

GUI::~GUI() {}

GUI::GuiType GUI::type() { return Type; }
