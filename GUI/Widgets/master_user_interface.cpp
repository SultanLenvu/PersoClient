#include "master_user_interface.h"
#include "production_manager_user_interface.h"
#include "programmator_user_interface.h"
#include "sticker_printer_user_interface.h"

MasterUserInterface::MasterUserInterface(QWidget* parent) : QWidget(parent) {
  create();
  connectDepedencies();
}

void MasterUserInterface::create() {
  MainLayout = new QHBoxLayout();
  setLayout(MainLayout);

  // Вкладки с всеми интерфейсами
  Tabs = new QTabWidget();
  MainLayout->addWidget(Tabs);

  // Интерфейс для взаимодействия с сервером персонализации
  createProductionManagerTab();

  // Интерфейс для взаимодействия с программатором
  createProgrammatorTab();

  // Интерфейс для взаимодействия с принтером стикером
  createStickerPrinterTab();

  // Виджеты для отображения логов
  createLogWidgets();

  // Подключаем внешние зависимости
  connectDepedencies();

  // Настройка пропорции между объектами на макете
  MainLayout->setStretch(0, 5);
  MainLayout->setStretch(1, 2);
}

void MasterUserInterface::createProductionManagerTab() {
  ProductionManagerTab = new ProductionManagerUserInterface();
  Tabs->addTab(ProductionManagerTab, "Производство");
}

void MasterUserInterface::createProgrammatorTab() {
  ProgrammatorTab = new ProgrammatorUserInterface();
  Tabs->addTab(ProgrammatorTab, "Программатор");
}

void MasterUserInterface::createStickerPrinterTab() {
  StickerPrinterTab = new StickerPrinterUserInterface();
  Tabs->addTab(StickerPrinterTab, "Стикер принтер");
}

void MasterUserInterface::createLogWidgets() {
  LogDisplay = new QGroupBox("Лог");
  MainLayout->addWidget(LogDisplay);
}

void MasterUserInterface::connectDepedencies() {
}
