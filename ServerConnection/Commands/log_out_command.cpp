#include "log_out_command.h"
#include "Management/global_environment.h"
#include "ProductionDispatcher/abstract_production_dispatcher.h"

LogOutCommand::LogOutCommand(const QString& name)
    : AbstractClientCommand(name) {
  Status = ReturnStatus::Unknown;

  connect(
      this, &LogOutCommand::logOut_signal,
      dynamic_cast<AbstractProductionDispatcher*>(
          GlobalEnvironment::instance()->getObject("GeneralProductionDispatcher")),
      &AbstractProductionDispatcher::shutdownProductionLine,
      Qt::BlockingQueuedConnection);
}

LogOutCommand::~LogOutCommand() {}

void LogOutCommand::process(const QJsonObject& command) {
  if (command.size() != Size ||
      (command["command_name"] != Name) || !command.contains("login") ||
      !command.contains("password")) {
    Status = ReturnStatus::SyntaxError;
    return;
  }

  Parameters.insert("login", command.value("login").toString());
  Parameters.insert("password", command.value("password").toString());

  // Запрашиваем печать бокса
  emit logOut_signal(Parameters, Status);
}

void LogOutCommand::generateResponse(QJsonObject& response) {
  response["response_name"] = Name;
  response["return_status"] = QString::number(static_cast<size_t>(Status));
}

void LogOutCommand::reset() {
  Parameters.clear();
  Status = ReturnStatus::Unknown;
}
