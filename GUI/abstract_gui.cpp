#include "abstract_gui.h"

AbstractGui::AbstractGui(QWidget* parent, GuiType type) : QWidget(parent) {
  Type = type;

  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);
}

AbstractGui::~AbstractGui() {
  //  qDebug() << "Deleted.";
}

AbstractGui::GuiType AbstractGui::type() {
  return Type;
}
