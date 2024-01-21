#include "abstract_gui.h"

AbstractGUI::AbstractGUI(QWidget* parent, GuiType type) : QWidget(parent)
{
  Type = type;

  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);
}

AbstractGUI::~AbstractGUI()
{
  //  qDebug() << "Deleted.";
}

AbstractGUI::GuiType AbstractGUI::type()
{
  return Type;
}
