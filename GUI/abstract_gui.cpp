#include "abstract_gui.h"

AbstractGui::AbstractGui(QWidget* parent) : QWidget(parent) {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);
}

AbstractGui::~AbstractGui() {
  //  qDebug() << "Deleted.";
}
