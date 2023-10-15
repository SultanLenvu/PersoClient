#include "interaction_system.h"

InteractionSystem::InteractionSystem(QWidget* parent)
    : QWidget(parent) {
  setObjectName("InteractionSystem");
  loadSettings();

  ProgressDialog = nullptr;
  CurrentOperationStep = 0;

  // Создаем таймеры
  createTimers();
}

InteractionSystem* InteractionSystem::instance() {
  static InteractionSystem interactor(nullptr);
  return &interactor;
}

void InteractionSystem::generateMessage(const QString& data) {
  QMessageBox::information(this, "Сообщение", data, QMessageBox::Ok);
}

void InteractionSystem::generateErrorMessage(const QString& text) {
  QMessageBox::critical(this, "Ошибка", text, QMessageBox::Ok);
}

void InteractionSystem::startOperationProgressDialog(
    const QString& operationName) {
  QSettings settings;

  // Создаем  окно
  createProgressDialog();

  //  Настраиваем и запускаем таймер для измерения квантов времени
  uint64_t operationDuration =
      settings.value(QString("duration_of_operations/") + operationName)
          .toInt();
  uint32_t operationQuantDuration = operationDuration / 100;
  operationQuantDuration += 1;
  sendLog(QString("Длительность кванта операции: %1.")
              .arg(QString::number(operationQuantDuration)));
  ODQTimer->setInterval(operationQuantDuration);
  ODQTimer->start();

  // Запускаем таймер для контроля максимальной длительности операции
  ODTimer->start();

  // Запускаем измеритель длительности операции
  ODMeter->start();
}

void InteractionSystem::finishOperationProgressDialog(
    const QString& operationName) {
  QSettings settings;

  // Измеряем и сохраняем длительность операции
  uint64_t duration = ODMeter->elapsed();
  sendLog(QString("Длительность операции: %1.").arg(QString::number(duration)));
  settings.setValue(QString("duration_of_operations/") + operationName,
                    QVariant::fromValue(duration));

  // Останавливаем таймер для контроля максимальной длительности операции
  ODTimer->stop();

  // Останавливаем для измерения кванта длительности операции
  ODQTimer->stop();

  // Закрываем окно
  destroyProgressDialog();
}

void InteractionSystem::getMasterPassword(QString& pass) {
  pass = QInputDialog::getText(this, "Мастер доступ",
                               "Введите пароль:", QLineEdit::Password, "",
                               nullptr);
}

void InteractionSystem::getAuthorizationData(QMap<QString, QString>* data) {
  AuthorizationDialog* dialog = new AuthorizationDialog(this);

  if (dialog->exec() == QDialog::Accepted) {
    dialog->getData(data);
  }
}

void InteractionSystem::getTransponderStickerData(QStringList* data,
                                                      bool& ok) {
  TransponderStickerScanDialog* dialog = new TransponderStickerScanDialog(this);

  if (dialog->exec() == QDialog::Accepted) {
    dialog->getData(data);
    ok = true;
  } else {
    ok = false;
  }
}

void InteractionSystem::getCustomTransponderStickerData(
    QMap<QString, QString>* data,
    bool& ok) {
  CustomTransponderStickerScanDialog* dialog =
      new CustomTransponderStickerScanDialog(this);

  if (dialog->exec() == QDialog::Accepted) {
    dialog->getData(data);
    ok = true;
  } else {
    ok = false;
  }
}

void InteractionSystem::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();
}

/*
 * Приватные методы
 */

void InteractionSystem::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("user_interaction_system/log_enable").toBool();
}

void InteractionSystem::sendLog(const QString& log) {
  if (LogEnable) {
    emit logging("InteractionSystem - " + log);
  }
}

void InteractionSystem::createProgressDialog() {
  ProgressDialog =
      new QProgressDialog("Выполнение операции...", "Закрыть", 0, 100);
  ProgressDialog->setWindowModality(Qt::ApplicationModal);
  ProgressDialog->setAutoClose(false);
  ProgressDialog->show();
}

void InteractionSystem::destroyProgressDialog() {
  ProgressDialog->close();
  delete ProgressDialog;
  ProgressDialog = nullptr;
  CurrentOperationStep = 0;
}

void InteractionSystem::createTimers() {
  // Таймер, отслеживающий длительность выполняющихся операций
  ODTimer = new QTimer(this);
  ODTimer->setInterval(CLIENT_MANAGER_OPERATION_MAX_DURATION);
  connect(ODTimer, &QTimer::timeout, this,
          &InteractionSystem::on_ODTimerTimeout_slot);
  connect(ODTimer, &QTimer::timeout, ODTimer, &QTimer::stop);

  // Таймер для измерения длительности операции
  ODMeter = new QElapsedTimer();

  // Таймер, отслеживающий квант длительности операции
  ODQTimer = new QTimer(this);
  connect(ODQTimer, &QTimer::timeout, this,
          &InteractionSystem::on_ODQTimerTimeout_slot);
}

void InteractionSystem::on_ODTimerTimeout_slot() {
  sendLog("Операция выполняется слишком долго. Сброс. ");
  generateErrorMessage("Операция выполняется слишком долго. Сброс. ");
}

void InteractionSystem::on_ODQTimerTimeout_slot() {
  if (!ProgressDialog) {
    return;
  }

  CurrentOperationStep++;
  if (CurrentOperationStep < 100) {
    ProgressDialog->setValue(CurrentOperationStep);
  }
}
