#include "master_user_interface.h"
#include "log_display_widget.h"
#include "production_manager_widget.h"
#include "programmator_widget.h"
#include "sticker_printer_widget.h"

MasterUserInterface::MasterUserInterface(QWidget* parent) : QWidget(parent) {
  create();
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

  // Настройка пропорции между объектами на макете
  MainLayout->setStretch(0, 5);
  MainLayout->setStretch(1, 2);
}

void MasterUserInterface::createProductionManagerTab() {
  ProductionManagerTab = new ProductionManagerWidget();
  Tabs->addTab(ProductionManagerTab, "Производство");
}

void MasterUserInterface::createProgrammatorTab() {
  ProgrammatorTab = new ProgrammatorWidget();
  Tabs->addTab(ProgrammatorTab, "Программатор");
}

void MasterUserInterface::createStickerPrinterTab() {
  StickerPrinterTab = new StickerPrinterWidget();
  Tabs->addTab(StickerPrinterTab, "Стикер принтер");
}

void MasterUserInterface::createLogWidgets() {
  LogDisplay = new LogDisplayWidget();
  MainLayout->addWidget(LogDisplay);
}
