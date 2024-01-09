#include <QSettings>

#include "definitions.h"
#include "interaction_system.h"

InteractionSystem::InteractionSystem(const QString& name) : QObject(nullptr) {
  setObjectName(name);
  loadSettings();

  // Создаем таймеры
  createTimers();
}

void InteractionSystem::generateMessage(const QString& data) {
  QMessageBox::information(nullptr, "Сообщение", data, QMessageBox::Ok);
}

void InteractionSystem::generateErrorMessage(const QString& text) {
  QMessageBox::critical(nullptr, "Ошибка", text, QMessageBox::Ok);
}

void InteractionSystem::operationStarted(const QString& operationName) {
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

void InteractionSystem::operationFinished(const QString& operationName,
                                          ReturnStatus ret) {
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

void InteractionSystem::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();
}

/*
 * Приватные методы
 */

void InteractionSystem::loadSettings() {
  QSettings settings;
}

void InteractionSystem::sendLog(const QString& log) {
  emit logging(objectName() + " - " + log);
}

void InteractionSystem::createProgressDialog() {
  ProgressDialog = std::unique_ptr<QProgressDialog>(
      new QProgressDialog("Выполнение операции...", "Закрыть", 0, 100));
  ProgressDialog->setWindowModality(Qt::ApplicationModal);
  ProgressDialog->setAutoClose(false);
  ProgressDialog->show();
}

void InteractionSystem::destroyProgressDialog() {
  ProgressDialog->close();
  ProgressDialog.reset();
}

void InteractionSystem::createTimers() {
  // Таймер, отслеживающий длительность выполняющихся операций
  ODTimer = std::make_unique<QTimer>(new QTimer());
  ODTimer->setInterval(CLIENT_MANAGER_OPERATION_MAX_DURATION);
  connect(ODTimer.get(), &QTimer::timeout, this,
          &InteractionSystem::odTimerTimeout_slot);
  connect(ODTimer.get(), &QTimer::timeout, ODTimer.get(), &QTimer::stop);

  // Таймер для измерения длительности операции
  ODMeter = std::unique_ptr<QElapsedTimer>(new QElapsedTimer());

  // Таймер, отслеживающий квант длительности операции
  ODQTimer = std::make_unique<QTimer>(new QTimer());
  connect(ODQTimer.get(), &QTimer::timeout, this,
          &InteractionSystem::odqTimerTimeout_slot);
}

void InteractionSystem::odTimerTimeout_slot() {
  sendLog("Операция выполняется слишком долго. Сброс. ");
  generateErrorMessage("Операция выполняется слишком долго. Сброс. ");
}

void InteractionSystem::odqTimerTimeout_slot() {
  if (!ProgressDialog) {
    return;
  }

  uint32_t cv = ProgressDialog->value();
  if (cv < 100) {
    ProgressDialog->setValue(++cv);
  }
}
