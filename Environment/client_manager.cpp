#include "client_manager.h"

ClientManager::ClientManager(QObject* parent) : QObject(parent) {
  setObjectName("ClientManager");
  loadSettings();

  // Создаем программатор
  createProgrammerInstance();

  // Создаем клиент
  createClientInstance();

  // Создаем принтер
  createStickerPrinterInstance();

  CurrentLogin = PRODUCTION_LINE_DEFAULT_LOGIN;
  CurrentPassword = PRODUCTION_LINE_DEFAULT_PASSWORD;
}

ClientManager::~ClientManager() {
}

void ClientManager::on_InsctanceThreadStarted_slot() {}

void ClientManager::performServerConnecting() {
  startOperationPerforming("performServerConnecting");

  sendLog("Подключение к серверу персонализации. ");
  if (Client->connectToServer() != PersoClient::Completed) {
    finishOperationPerforming("performServerConnecting");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performServerConnecting");
}

void ClientManager::performServerDisconnecting() {
  startOperationPerforming("performServerDisconnecting");

  sendLog("Отключение от сервера персонализации. ");
  PersoClient::ReturnStatus status = Client->disconnectFromServer();
  if (status != PersoClient::Completed) {
    processClientError(status, "performServerDisconnecting");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performServerDisconnecting");
}

void ClientManager::performServerEcho() {
  startOperationPerforming("performServerEcho");

  sendLog("Отправка эхо-запроса на сервер персонализации. ");
  PersoClient::ReturnStatus status = Client->requestEcho();
  if (status != PersoClient::Completed) {
    processClientError(status, "performServerEcho");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performServerEcho");
}

void ClientManager::performServerAuthorization(
    const QSharedPointer<QHash<QString, QString>> data) {
  startOperationPerforming("performServerAuthorization");

  sendLog("Авторизация на сервере персонализации. ");
  PersoClient::ReturnStatus status = Client->requestAuthorize(data.get());
  if (status != PersoClient::Completed) {
    processClientError(status, "performServerAuthorization");
    return;
  }

  // Сохраняем данные и вызываем производственный интерфейс
  CurrentLogin = data->value("login");
  CurrentPassword = data->value("password");

  // Запрашиваем создание производственного интерфейса
  emit requestProductionInterface_signal();

  // Завершаем операцию
  finishOperationPerforming("performServerAuthorization");
}

void ClientManager::performTransponderFirmwareLoading() {
  startOperationPerforming("performTransponderFirmwareLoading");

  IProgrammer::ReturnStatus programmerStatus;
  PersoClient::ReturnStatus clientStatus;
  IStickerPrinter::ReturnStatus stickerPrinterStatus;
  QString ucid;
  QHash<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);
  QSharedPointer<QHash<QString, QString>> transponderData(
      new QHash<QString, QString>());

  sendLog("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  sendLog("Считывание UCID транспондера. ");
  programmerStatus = Programmer->getUcid(&ucid);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  sendLog("Запрос прошивки транспондера. ");
  clientStatus = Client->requestTransponderRelease(
      &requestParameters, &firmware, transponderData.get());
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareLoading");
    return;
  }
  requestParameters.clear();

  sendLog("Загрузка прошивки в транспондер. ");
  programmerStatus = Programmer->loadFirmware(&firmware);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  // Удаляем файл прошивки
  firmware.remove();

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(transponderData);

  sendLog("Печать стикера для транспондера.");
  stickerPrinterStatus =
      StickerPrinter->printTransponderSticker(transponderData.get());
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("ucid", ucid);
  sendLog(
      "Отправка запроса на подтверждение загрузки прошивки в транспондер. ");
  clientStatus = Client->requestTransponderReleaseConfirm(&requestParameters);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareLoading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performTransponderFirmwareLoading");
}

void ClientManager::performTransponderFirmwareReloading(const QString& pan) {
  startOperationPerforming("performTransponderFirmwareReloading");

  IProgrammer::ReturnStatus programmerStatus;
  PersoClient::ReturnStatus clientStatus;
  IStickerPrinter::ReturnStatus stickerPrinterStatus;
  QString ucid;
  QHash<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);
  QSharedPointer<QHash<QString, QString>> transponderData(
      new QHash<QString, QString>());

  sendLog("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  sendLog("Считывание UCID транспондера. ");
  programmerStatus = Programmer->getUcid(&ucid);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  sendLog("Запрос перевыпуска транспондера. ");
  clientStatus = Client->requestTransponderRerelease(
      &requestParameters, &firmware, transponderData.get());
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareReloading");
    return;
  }
  requestParameters.clear();

  sendLog("Загрузка прошивки в транспондер. ");
  programmerStatus = Programmer->loadFirmware(&firmware);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  // Удаляем файл прошивки
  firmware.remove();

  // Запрашиваем отображение данных транспондера
  emit displayTransponderData_signal(transponderData);

  sendLog("Печать стикера для транспондера.");
  stickerPrinterStatus =
      StickerPrinter->printTransponderSticker(transponderData.get());
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  requestParameters.insert("ucid", ucid);
  sendLog("Отправка запроса на подтверждение перевыпуска транспондера. ");
  clientStatus = Client->requestTransponderRereleaseConfirm(&requestParameters);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareReloading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performTransponderFirmwareReloading");
}

void ClientManager::rollbackProductionLine() {
  startOperationPerforming("rollbackProductionLine");
  sendLog("Откат производственной линии. ");

  QHash<QString, QString> param;
  param.insert("login", CurrentLogin);
  param.insert("password", CurrentPassword);
  PersoClient::ReturnStatus status =
      Client->requestProductionLineRollback(&param);
  if (status != PersoClient::Completed) {
    processClientError(status, "rollbackProductionLine");
    return;
  }

  finishOperationPerforming("rollbackProductionLine");
}

void ClientManager::performBoxStickerPrinting(
    const QSharedPointer<QHash<QString, QString>> param) {
  startOperationPerforming("printBoxStickerOnServer");
  sendLog("Запрос печати стикера для бокса на сервере. ");

  PersoClient::ReturnStatus status =
      Client->requestBoxStickerPrint(param.get());
  if (status != PersoClient::Completed) {
    processClientError(status, "printBoxStickerOnServer");
    return;
  }

  finishOperationPerforming("printBoxStickerOnServer");
}

void ClientManager::performPalletStickerPrinting(
    const QSharedPointer<QHash<QString, QString>> param) {
  startOperationPerforming("printPalletStickerOnServer");
  sendLog("Запрос печати стикера для паллеты на сервере. ");

  PersoClient::ReturnStatus status =
      Client->requestPalletStickerPrint(param.get());
  if (status != PersoClient::Completed) {
    processClientError(status, "printPalletStickerOnServer");
    return;
  }

  finishOperationPerforming("printPalletStickerOnServer");
}

void ClientManager::performLocalFirmwareLoading(const QString& path) {
  startOperationPerforming("performLocalFirmwareLoading");

  IProgrammer::ReturnStatus programmerStatus;
  QFile firmware(path);

  sendLog("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performLocalFirmwareLoading");
    return;
  }

  sendLog("Загрузка прошивки в транспондер. ");
  programmerStatus = Programmer->loadFirmware(&firmware);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performLocalFirmwareLoading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performLocalFirmwareLoading");
}

void ClientManager::performFirmwareReading() {
  startOperationPerforming("performFirmwareReading");

  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Считывание прошивки микроконтроллера. ");
  programmerStatus = Programmer->readFirmware();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performFirmwareReading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performFirmwareReading");
}

void ClientManager::performFirmwareErasing() {
  startOperationPerforming("performFirmwareErasing");

  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Стирание прошивки микроконтроллера. ");
  programmerStatus = Programmer->eraseFirmware();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performFirmwareErasing");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performFirmwareErasing");
}

void ClientManager::performDataReading() {
  startOperationPerforming("performDataReading");

  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Считывание данных из памяти микроконтроллера. ");
  programmerStatus = Programmer->readData();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performDataReading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performDataReading");
}

void ClientManager::performDataLoading(const QString& path) {
  startOperationPerforming("performDataLoading");

  // Создаем файл с данными
  QFile dataFile(path);
  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Загрузка данных в память микроконтроллера. ");
  programmerStatus = Programmer->loadData(&dataFile);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performDataLoading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performDataLoading");
}

void ClientManager::performDeviceUnlock() {
  startOperationPerforming("performDeviceUnlock");

  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Разблокирование памяти микроконтроллера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performDeviceUnlock");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performDeviceUnlock");
}

void ClientManager::performDeviceLock() {
  startOperationPerforming("performDeviceLock");

  IProgrammer::ReturnStatus programmerStatus;

  sendLog("Блокирование памяти микроконтроллера. ");
  programmerStatus = Programmer->lockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performDeviceUnlock");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performDeviceLock");
}

void ClientManager::performLastTransponderStickerPrinting() {
  startOperationPerforming("performLastTransponderStickerPrinting");

  IStickerPrinter::ReturnStatus stickerPrinterStatus;

  sendLog("Печать последнего стикера транспондера. ");
  stickerPrinterStatus = StickerPrinter->printLastTransponderSticker();
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performLastTransponderStickerPrinting");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performLastTransponderStickerPrinting");
}

void ClientManager::performCustomTransponderStickerPrinting(
    const QSharedPointer<QHash<QString, QString>> data) {
  startOperationPerforming("performStickerPrinterCommandScript");

  IStickerPrinter::ReturnStatus stickerPrinterStatus;

  sendLog("Печать произвольного стикера транспондера. ");
  stickerPrinterStatus = StickerPrinter->printTransponderSticker(data.get());
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performStickerPrinterCommandScript");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performStickerPrinterCommandScript");
}

void ClientManager::performStickerPrinterCommandScript(
    const QSharedPointer<QStringList> commandScript) {
  startOperationPerforming("performStickerPrinterCommandScript");

  IStickerPrinter::ReturnStatus stickerPrinterStatus;

  sendLog("Выполнение командного скрипта для принтера. ");
  stickerPrinterStatus = StickerPrinter->exec(commandScript.get());
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performStickerPrinterCommandScript");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performStickerPrinterCommandScript");
}

void ClientManager::applySettings() {
  sendLog("Применение новых настроек. ");
  loadSettings();

  Client->applySettings();
  Programmer->applySettings();
  StickerPrinter->applySetting();
}

void ClientManager::loadSettings() {
  QSettings settings;

  LogEnable = settings.value("log_system/global_enable").toBool();
}

void ClientManager::sendLog(const QString& log) const {
  if (LogEnable) {
    emit logging(QString("%1 - %2").arg(objectName(), log));
  }
}

void ClientManager::createProgrammerInstance() {
  Programmer = new JLinkExeProgrammer(this);
  connect(Programmer, &IProgrammer::logging, LogSystem::instance(),
          &LogSystem::generate);

  // Заполняем таблицу соответствий статусов возврата
  ProgrammerReturnStatusMatch.insert(IProgrammer::NotExecuted,
                                     "Выполнение операции не началось.");
  ProgrammerReturnStatusMatch.insert(IProgrammer::FirmwareFileError,
                                     "Некорректный файл прошивки. ");
  ProgrammerReturnStatusMatch.insert(IProgrammer::DataFileError,
                                     "Некорректный файл данных. ");
  ProgrammerReturnStatusMatch.insert(IProgrammer::ProgrammatorError,
                                     "Ошибка программатора.");
  ProgrammerReturnStatusMatch.insert(IProgrammer::Completed, "Выполнено.");
}

void ClientManager::createClientInstance() {
  Client = new PersoClient(this);
  connect(Client, &PersoClient::logging, LogSystem::instance(),
          &LogSystem::generate);

  // Заполняем таблицу соответствий статусов возврата
  ClientReturnStatusMatch.insert(PersoClient::Completed, "Выполнено. ");
  ClientReturnStatusMatch.insert(PersoClient::RequestParameterError,
                                 "Не удалось обработать параметры запроса. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionError,
                                 "Не удалось подключиться к серверу. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerNotResponding,
                                 "Сервер не отвечает.");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionTerminated,
                                 "Оборвалось соединение с сервером.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotExist,
                                 "Производственная линия не найдена. ");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationAccessDenied,
                                 "Ошибка доступа к производственной линии.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotActive,
                                 "Производственная линия не активна. ");
  ClientReturnStatusMatch.insert(PersoClient::ResponseSyntaxError,
                                 "Синтаксическая ошибка в ответе на запрос. ");
  ClientReturnStatusMatch.insert(
      PersoClient::CommandSyntaxError,
      "В серверном запросе допущена синтаксическая ошибка. ");
  ClientReturnStatusMatch.insert(
      PersoClient::DatabaseError,
      "Получена ошибка при выполнении запроса в базу данных. ");
  ClientReturnStatusMatch.insert(PersoClient::TransponderNotFound,
                                 "Транспондер не найден.");
  ClientReturnStatusMatch.insert(
      PersoClient::TransponderNotReleasedEarlier,
      "Транспондер не был выпущен ранее, перевыпуск невозможен. ");
  ClientReturnStatusMatch.insert(PersoClient::AwaitingConfirmationError,
                                 "Транспондер не ожидает подтверждения. ");
  ClientReturnStatusMatch.insert(PersoClient::IdenticalUcidError,
                                 "Используется одна и та же печатная плата. "
                                 "Перевыпуск транспондера невозможен. ");
  ClientReturnStatusMatch.insert(PersoClient::ProductionLineMissed,
                                 "Производственная линия не найдена. ");
  ClientReturnStatusMatch.insert(PersoClient::ProductionLineNotActive,
                                 "Производственная линия не активна. ");
  ClientReturnStatusMatch.insert(
      PersoClient::CurrentOrderRunOut,
      "В текущем заказе отсутствуют свободные транспондеры . ");
  ClientReturnStatusMatch.insert(PersoClient::CurrentOrderAssembled,
                                 "Текущий заказ собран. ");
  ClientReturnStatusMatch.insert(PersoClient::ProductionLineRollbackLimitError,
                                 "Производственная линия связана с первым "
                                 "транспондером в боксе. Откат невозможен. ");
  ClientReturnStatusMatch.insert(PersoClient::BoxStickerPrintError,
                                 "Не удалось распечатать стикер для бокса. ");
  ClientReturnStatusMatch.insert(PersoClient::PalletStickerPrintError,
                                 "Не удалось распечатать стикер для паллеты. ");
  ClientReturnStatusMatch.insert(
      PersoClient::NextTransponderNotFound,
      "Получена ошибка при поиске очередного транспондера. ");
  ClientReturnStatusMatch.insert(
      PersoClient::StartBoxAssemblingError,
      "Получена ошибка при запуске сборки очередного бокса. ");
  ClientReturnStatusMatch.insert(
      PersoClient::StartPalletAssemblingError,
      "Получена ошибка при запуске сборки очередной паллеты. ");
}

void ClientManager::createStickerPrinterInstance() {
  StickerPrinter = new TE310Printer(this, "TSC TE310");
  connect(StickerPrinter, &IStickerPrinter::logging, LogSystem::instance(),
          &LogSystem::generate);

  // Заполняем таблицу соответствий статусов возврата
  StickerPrinterReturnStatusMatch.insert(
      IStickerPrinter::ParameterError,
      "Получены некорректные параметры для стикера.");
  StickerPrinterReturnStatusMatch.insert(IStickerPrinter::Failed,
                                         "Не удалось распечать стикер.");
  StickerPrinterReturnStatusMatch.insert(
      IStickerPrinter::LibraryMissed,
      "Отсутствует библиотека для работы с принтером стикеров.");
  StickerPrinterReturnStatusMatch.insert(IStickerPrinter::ConnectionError,
                                         "Принтер стикеров не подключен.");
  StickerPrinterReturnStatusMatch.insert(IStickerPrinter::Completed,
                                         "Выполнено.");
}

void ClientManager::startOperationPerforming(const QString& operationName) {
  // Блокируем мьютекс
  Mutex.lock();

  // Отправляем сигнал о начале выполнения длительной операции
  emit operationPerfomingStarted(operationName);
}

void ClientManager::finishOperationPerforming(const QString& operationName) {
  // Сигнал о завершении текущей операции
  emit operationPerformingFinished(operationName);

  // Опопвещаем пользователя
  emit notifyUser("Выполнено.");

  // Разблокируем мьютекс
  Mutex.unlock();
}

void ClientManager::processClientError(PersoClient::ReturnStatus status,
                                       const QString& operationName) {
  emit operationPerformingFinished(operationName);
  emit notifyUserAboutError(ClientReturnStatusMatch.value(status));
  Mutex.unlock();
}

void ClientManager::processProgrammerError(IProgrammer::ReturnStatus status,
                                           const QString& operationName) {
  sendLog(ProgrammerReturnStatusMatch.value(status));
  emit operationPerformingFinished(operationName);
  emit notifyUserAboutError(ProgrammerReturnStatusMatch.value(status));
  Mutex.unlock();
}

void ClientManager::processStickerPrintersError(
    IStickerPrinter::ReturnStatus status,
    const QString& operationName) {
  sendLog(StickerPrinterReturnStatusMatch.value(status));
  emit operationPerformingFinished(operationName);
  emit notifyUserAboutError(StickerPrinterReturnStatusMatch.value(status));
  Mutex.unlock();
}
