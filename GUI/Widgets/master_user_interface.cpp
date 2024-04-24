#include "master_user_interface.h"

MasterUserInterface::MasterUserInterface(QWidget* pmw,
                                         QWidget* pw,
                                         QWidget* spw,
                                         QWidget* ldw) {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  createTabs(pmw, pw, spw);

  MainLayout->addWidget(ldw);

  MainLayout->setStretch(0, 5);
  MainLayout->setStretch(1, 2);
}

void MasterUserInterface::createTabs(QWidget* pmw, QWidget* pw, QWidget* spw) {
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  Tabs->addTab(pmw, "Производство");
  Tabs->addTab(pw, "Программатор");
  Tabs->addTab(spw, "Стикер принтер");
}
