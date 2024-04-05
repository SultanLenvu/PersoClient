#include "server_commands_widget.h"
#include "global_environment.h"
#include "server_connection_gui_subkernel.h"

ServerCommandsWidget::ServerCommandsWidget(QWidget* parent)
    : QGroupBox{parent} {
  create();
  connectDependencies();
}

void ServerCommandsWidget::create() {
  setTitle("Клиентские запросы");

  Layout = new QVBoxLayout();
  setLayout(Layout);

  ComboBox = new QComboBox();
  Layout->addWidget(ComboBox);
  ComboBox->addItem("connect");
  ComboBox->addItem("disconnect");

  ComboBox->addItem("echo");
  ComboBox->addItem("launch_production_line");
  ComboBox->addItem("shutdown_production_line");
  ComboBox->addItem("get_production_line_data");

  ComboBox->addItem("request_box");
  ComboBox->addItem("get_current_box_data");
  ComboBox->addItem("refund_current_box");
  ComboBox->addItem("complete_current_box");

  ComboBox->addItem("release_transponder");
  ComboBox->addItem("confirm_transponder_release");
  ComboBox->addItem("rerelease_transponder");
  ComboBox->addItem("confirm_transponder_rerelease");
  ComboBox->addItem("rollback_tranponder");
  ComboBox->addItem("get_current_transponder_data");
  ComboBox->addItem("get_transponder_data");

  ComboBox->addItem("print_box_sticker");
  ComboBox->addItem("print_last_box_sticker");
  ComboBox->addItem("print_pallet_sticker");
  ComboBox->addItem("print_last_pallet_sticker");

  Button = new QPushButton("Выполнить");
  Layout->addWidget(Button);
  connect(Button, &QPushButton::clicked, this,
          &ServerCommandsWidget::prepareCommandExecution);
}

void ServerCommandsWidget::connectDependencies() {
  const ServerConnectionGuiSubkernel* sgs =
      GlobalEnvironment::instance()
          ->getObject<const ServerConnectionGuiSubkernel>(
              "ServerConnectionGuiSubkernel");

  QObject::connect(this, &ServerCommandsWidget::executeCommand_signal, sgs,
                   &ServerConnectionGuiSubkernel::executeCommand);
}

void ServerCommandsWidget::prepareCommandExecution() {
  emit executeCommand_signal(ComboBox->currentText());
}
