#include "client_manager.h"

ClientManager::ClientManager(QObject* parent) : QObject(parent) {
  setObjectName("ClientManager");

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

  emit logging("Подключение к серверу персонализации. ");
  if (Client->connectToServer() != PersoClient::Completed) {
    emit logging("");
    finishOperationPerforming("performServerConnecting");
  }

  // Завершаем операцию
  finishOperationPerforming("performServerConnecting");
}

void ClientManager::performServerDisconnecting() {
  startOperationPerforming("performServerDisconnecting");

  emit logging("Отключение от сервера персонализации. ");
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

  emit logging("Отправка эхо-запроса на сервер персонализации. ");
  PersoClient::ReturnStatus status = Client->requestEcho();
  if (status != PersoClient::Completed) {
    processClientError(status, "performServerEcho");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performServerEcho");
}

void ClientManager::performServerAuthorization(
    const QSharedPointer<QMap<QString, QString>> data) {
  startOperationPerforming("performServerAuthorization");

  emit logging("Авторизация на сервере персонализации. ");
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

void ClientManager::performTransponderFirmwareLoading(
    TransponderInfoModel* model) {
  startOperationPerforming("performTransponderFirmwareLoading");

  IProgrammer::ReturnStatus programmerStatus;
  PersoClient::ReturnStatus clientStatus;
  IStickerPrinter::ReturnStatus stickerPrinterStatus;
  QString ucid;
  QMap<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);

  emit logging("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  programmerStatus = Programmer->getUcid(&ucid);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  emit logging("Запрос прошивки транспондера. ");
  clientStatus =
      Client->requestTransponderRelease(&requestParameters, &firmware);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareLoading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  programmerStatus = Programmer->loadFirmware(&firmware);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareLoading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("ucid", ucid);
  emit logging(
      "Отправка запроса на подтверждение загрузки прошивки в транспондер. ");
  clientStatus = Client->requestTransponderReleaseConfirm(&requestParameters,
                                                          responseParameters);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareLoading");
    return;
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  firmware.remove();

  emit logging("Печать стикера для транспондера.");
  stickerPrinterStatus =
      StickerPrinter->printTransponderSticker(responseParameters);
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performTransponderFirmwareLoading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performTransponderFirmwareLoading");
}

void ClientManager::performTransponderFirmwareReloading(
    TransponderInfoModel* model,
    const QString& pan) {
  startOperationPerforming("performTransponderFirmwareReloading");

  IProgrammer::ReturnStatus programmerStatus;
  PersoClient::ReturnStatus clientStatus;
  IStickerPrinter::ReturnStatus stickerPrinterStatus;
  QString ucid;
  QMap<QString, QString> requestParameters;
  QFile firmware(FIRMWARE_TEMP_FILE_NAME, this);

  emit logging("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  emit logging("Считывание UCID транспондера. ");
  programmerStatus = Programmer->getUcid(&ucid);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  emit logging("Запрос перевыпуска транспондера. ");
  clientStatus =
      Client->requestTransponderRerelease(&requestParameters, &firmware);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareReloading");
    return;
  }
  requestParameters.clear();

  emit logging("Загрузка прошивки в транспондер. ");
  programmerStatus = Programmer->loadFirmware(&firmware);
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus,
                           "performTransponderFirmwareReloading");
    return;
  }

  QMap<QString, QString>* responseParameters = new QMap<QString, QString>;
  requestParameters.insert("login", CurrentLogin);
  requestParameters.insert("password", CurrentPassword);
  requestParameters.insert("pan", pan);
  requestParameters.insert("ucid", ucid);
  emit logging("Отправка запроса на подтверждение перевыпуска транспондера. ");
  clientStatus = Client->requestTransponderRereleaseConfirm(&requestParameters,
                                                            responseParameters);
  if (clientStatus != PersoClient::Completed) {
    processClientError(clientStatus, "performTransponderFirmwareReloading");
    return;
  }

  // Строим модель для представления данных транспондера
  model->build(responseParameters);

  // Удаляем файл прошивки
  firmware.remove();

  emit logging("Печать стикера для транспондера.");
  stickerPrinterStatus =
      StickerPrinter->printTransponderSticker(responseParameters);
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performTransponderFirmwareLoading");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performTransponderFirmwareReloading");
}

void ClientManager::performLocalFirmwareLoading(const QString& path) {
  startOperationPerforming("performLocalFirmwareLoading");

  IProgrammer::ReturnStatus programmerStatus;
  QFile firmware(path);

  emit logging("Разблокирование памяти транспондера. ");
  programmerStatus = Programmer->unlockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performLocalFirmwareLoading");
    return;
  }

  emit logging("Загрузка прошивки в транспондер. ");
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

  emit logging("Считывание прошивки микроконтроллера. ");
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

  emit logging("Стирание прошивки микроконтроллера. ");
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

  emit logging("Считывание данных из памяти микроконтроллера. ");
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

  emit logging("Загрузка данных в память микроконтроллера. ");
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

  emit logging("Разблокирование памяти микроконтроллера. ");
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

  emit logging("Блокирование памяти микроконтроллера. ");
  programmerStatus = Programmer->lockDevice();
  if (programmerStatus != IProgrammer::Completed) {
    processProgrammerError(programmerStatus, "performDeviceUnlock");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performDeviceLock");
}

void ClientManager::performPrintingLastTransponderSticker() {
  startOperationPerforming("performPrintingLastTransponderSticker");

  IStickerPrinter::ReturnStatus stickerPrinterStatus;

  emit logging("Печать последнего стикера транспондера. ");
  stickerPrinterStatus = StickerPrinter->printLastTransponderSticker();
  if (stickerPrinterStatus != IStickerPrinter::Completed) {
    processStickerPrintersError(stickerPrinterStatus,
                                "performPrintingLastTransponderSticker");
    return;
  }

  // Завершаем операцию
  finishOperationPerforming("performPrintingLastTransponderSticker");
}

void ClientManager::performPrintingCustomTransponderSticker(
    const QSharedPointer<QMap<QString, QString>> data) {
  startOperationPerforming("performStickerPrinterCommandScript");

  IStickerPrinter::ReturnStatus stickerPrinterStatus;

  emit logging("Печать произвольного стикера транспондера. ");
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

  emit logging("Выполнение командного скрипта для принтера. ");
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
  emit logging("Применение новых настроек. ");
  loadSettings();

  Client->applySettings();
  Programmer->applySettings();
  StickerPrinter->applySetting();
}

void ClientManager::loadSettings() {}

void ClientManager::createProgrammerInstance() {
  Programmer = new JLinkExeProgrammer(this);
  connect(Programmer, &IProgrammer::logging, this,
          &ClientManager::proxyLogging);

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
  connect(Client, &PersoClient::logging, this, &ClientManager::proxyLogging);

  // Заполняем таблицу соответствий статусов возврата
  ClientReturnStatusMatch.insert(PersoClient::NotExecuted,
                                 "Выполнение операции не началось.");
  ClientReturnStatusMatch.insert(PersoClient::RequestParameterError,
                                 "Не удалось обработать параметры запроса. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionError,
                                 "Не удалось подключиться к серверу. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerNotResponding,
                                 "Сервер не отвечает.");
  ClientReturnStatusMatch.insert(PersoClient::ServerConnectionTerminated,
                                 "Оборвалось соединение с сервером.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotExist,
                                 "Производственная линия не существует.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationAccessDenied,
                                 "Ошибка доступа к производственной линии.");
  ClientReturnStatusMatch.insert(PersoClient::AuthorizationNotActive,
                                 "Производственная линия не активна. ");
  ClientReturnStatusMatch.insert(PersoClient::ResponseSyntaxError,
                                 "Синтаксическая ошибка в ответе на запрос. ");
  ClientReturnStatusMatch.insert(PersoClient::ServerError,
                                 "Серверная ошибка. ");
  ClientReturnStatusMatch.insert(PersoClient::UnknownError,
                                 "Неизвествная ошибка. ");
  ClientReturnStatusMatch.insert(PersoClient::Completed, "Выполнено. ");
}

void ClientManager::createStickerPrinterInstance() {
  StickerPrinter = new TE310Printer(this);
  connect(StickerPrinter, &IStickerPrinter::logging, this,
          &ClientManager::proxyLogging);

  // Заполняем таблицу соответствий статусов возврата
  StickerPrinterReturnStatusMatch.insert(
      IStickerPrinter::ParameterError,
      "Получены некорректные параметры для стикера.");
  StickerPrinterReturnStatusMatch.insert(IStickerPrinter::Failed,
                                         "Не удалось распечать стикер.");
  StickerPrinterReturnStatusMatch.insert(
      IStickerPrinter::LibraryMissing,
      "Отсутствует библиотека для работы с принтером стикеров.");
  StickerPrinterReturnStatusMatch.insert(IStickerPrinter::NotConnected,
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
  emit operationPerformingFinished(operationName);
  emit notifyUserAboutError(ProgrammerReturnStatusMatch.value(status));
  Mutex.unlock();
}

void ClientManager::processStickerPrintersError(
    IStickerPrinter::ReturnStatus status,
    const QString& operationName) {
  emit operationPerformingFinished(operationName);
  emit notifyUserAboutError(StickerPrinterReturnStatusMatch.value(status));
  Mutex.unlock();
}

void ClientManager::proxyLogging(const QString& log) {
  if (sender()->objectName() == "IProgrammer")
    emit logging("Programmer - " + log);
  else if (sender()->objectName() == "PersoClient")
    emit logging("Client - " + log);
  else if (sender()->objectName() == "TE310Printer")
    emit logging("StickerPrinter - " + log);
  else
    emit logging("Unknown - " + log);
}
