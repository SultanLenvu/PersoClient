#include <QSettings>

#include "definitions.h"
#include "global_environment.h"
#include "interaction_system.h"
#include "log_system.h"

InteractionSystem::InteractionSystem(const QString& name) : QObject(nullptr) {
  setObjectName(name);
  loadSettings();

  // Создаем таймеры
  createTimers();

  // Создаем таблицу сообщений
  createMessageTable();

  GlobalEnvironment::instance()->registerObject(this);
  connect(this, &InteractionSystem::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}

InteractionSystem::~InteractionSystem() {}

void InteractionSystem::generateMessage(const QString& text) {
  QMessageBox::information(nullptr, "Сообщение", text, QMessageBox::Ok);
}

void InteractionSystem::generateErrorMessage(const QString& text) {
  QMessageBox::critical(nullptr, "Ошибка", text, QMessageBox::Ok);
}

void InteractionSystem::processOperationStart(const QString& operationName) {
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

void InteractionSystem::processOperationFinish(const QString& operationName,
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

  // Обрабатываем статус возврата
  processReturnStatus(ret);
}

void InteractionSystem::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();
}

/*
 * Приватные методы
 */

InteractionSystem::InteractionSystem() {}

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
  ProgressDialog->setValue(0);
  ProgressDialog->show();
}

void InteractionSystem::destroyProgressDialog() {
  ProgressDialog->close();

  /* !!!
   * Если здесь очищать помять под ProgressDialog, то происходит
   * необъяснимый вылет, почему это так, я не смог разобраться.
   * Вылет происходит при завершении какой-либо операции.
   */
  //  ProgressDialog.reset();
  //  sendLog("PD destroyed.");
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

void InteractionSystem::createMessageTable() {
  MessageTable[ReturnStatus::NoError] = "Выполнено.";
  MessageTable[ReturnStatus::DynamicLibraryMissing] =
      "Не найдена динамическая библиотека.";
  MessageTable[ReturnStatus::ParameterError] = "Получена ошибка параметра.";
  MessageTable[ReturnStatus::SyntaxError] = "Получена синтаксическая ошибка.";
  MessageTable[ReturnStatus::ConsistencyViolation] =
      "Получена ошибка синхронизации.";
  MessageTable[ReturnStatus::FileOpenError] =
      "Получена ошибка при открытии файла.";
  MessageTable[ReturnStatus::InvalidFirmwareFile] =
      "Получен некорректный файл прошивки.";

  MessageTable[ReturnStatus::ServerConnectionError] =
      "Не удалось подключиться к серверу.";
  MessageTable[ReturnStatus::ServerNotResponding] = "Сервер не отвечает.";
  MessageTable[ReturnStatus::ServerDataTransmittingError] =
      "Получена ошибка при отправке данных.";
  MessageTable[ReturnStatus::ServerResponseSyntaxError] =
      "Получена синтаксическая ошибка при обработке ответа от сервера.";
  MessageTable[ReturnStatus::ServerResponseDataBlockError] =
      "Получена ошибка обработке блока данных от сервера.";
  MessageTable[ReturnStatus::ServerResponseProcessingError] =
      "Получена ошибка при обработке ответа от сервера.";

  MessageTable[ReturnStatus::ClientCommandParamError] =
      "Получена ошибка в параметре клиентской комаенды.";
  MessageTable[ReturnStatus::ServerInternalError] =
      "Получена серверная ошибка.";

  MessageTable[ReturnStatus::ClientCommandParamError] =
      "Получена ошибка в параметре клиентской комаенды.";
  MessageTable[ReturnStatus::ServerInternalError] =
      "Получена серверная ошибка.";

  MessageTable[ReturnStatus::ProductionLineMissed] =
      "Производственная линия не найдена.";
  MessageTable[ReturnStatus::TranspoderMissed] = "Транспондер не найден.";
  MessageTable[ReturnStatus::BoxMissed] = "Бокс не найден.";
  MessageTable[ReturnStatus::PalletMissed] = "Паллета не найдена.";
  MessageTable[ReturnStatus::OrderMissed] = "Заказ не найден.";
  MessageTable[ReturnStatus::MasterKeysMissed] =
      "Ключи безопасности не найдены.";

  MessageTable[ReturnStatus::OrderMultiplyAssembly] =
      "Несколько заказов одновременно находятся в процессе сборки.";
  MessageTable[ReturnStatus::OrderInProcessMissed] =
      "Отсутствуют заказы в процессе сборки.";

  MessageTable[ReturnStatus::BoxAlreadyRequested] =
      "Бокс был получен ранее и находится в процессе сборки.";
  MessageTable[ReturnStatus::BoxNotRequested] = "Бокс не был запрошен ранее.";
  MessageTable[ReturnStatus::FreeBoxMissed] =
      "Не найден свободный бокс для сборки.";
  MessageTable[ReturnStatus::BoxCompletelyAssembled] =
      "Все транспондеры в боксе собраны.";
  MessageTable[ReturnStatus::BoxNotCompletelyAssembled] =
      "В боксе собраны не все транспондеры.";

  MessageTable[ReturnStatus::TransponderIncorrectRerelease] =
      "Получена ошибка при перевыпуске транспондера.";
  MessageTable[ReturnStatus::IdenticalUcidError] =
      "Получена ошибка идентификатора печатной платы.";
  MessageTable[ReturnStatus::CurrentOrderAssembled] =
      "Сборка текущего заказа успешно завершена.";

  MessageTable[ReturnStatus::ProductionLineContextNotAuthorized] =
      "Контекст производственной линии не авторизирован.";
  MessageTable[ReturnStatus::ProductionLineNotLaunched] =
      "Производственная линия не запущена.";
  MessageTable[ReturnStatus::ProductionLineLaunchError] =
      "Не удалось запустить производственную линию.";
  MessageTable[ReturnStatus::ProductionLineAlreadyLaunched] =
      "Производственная линия уже запущена.";
  MessageTable[ReturnStatus::ProductionLineNotActive] =
      "Производственная линия не активирована.";
  MessageTable[ReturnStatus::ProductionLineCompleted] =
      "Производственная линия завершила свою работу.";
  MessageTable[ReturnStatus::ProductionLineNotInProcess] =
      "Производственная линия не находится в процессе сборки. Необходимо "
      "запросить бокс.";
  MessageTable[ReturnStatus::ProductionLineRollbackLimit] =
      "Достигнут лимит отката для производственной линии.";

  MessageTable[ReturnStatus::StickerPrinterInitError] =
      "Не удалось инициализировать принтер стикеров.";
  MessageTable[ReturnStatus::StickerPrinterConnectionError] =
      "Не удалось подключиться к принтеру стикеров.";

  MessageTable[ReturnStatus::ProgrammatorError] =
      "Получена ошибка программатора.";
}

void InteractionSystem::processReturnStatus(ReturnStatus ret) {
  if (ret == ReturnStatus::NoError) {
    QMessageBox::information(nullptr, "Сообщение", MessageTable[ret],
                             QMessageBox::Ok);
  } else {
    QMessageBox::critical(nullptr, "Ошибка", MessageTable[ret],
                          QMessageBox::Ok);
  }
}

void InteractionSystem::odTimerTimeout_slot() {
  sendLog("Операция выполняется слишком долго. Сброс. ");
  generateErrorMessage("Операция выполняется слишком долго. Сброс. ");
}

void InteractionSystem::odqTimerTimeout_slot() {
  //  if (!ProgressDialog) {
  //    return;
  //  }

  uint32_t cv = ProgressDialog->value();
  //  sendLog(QString("quant %1").arg(QString::number(cv)));
  if (cv < 100) {
    ProgressDialog->setValue(++cv);
  }
}
