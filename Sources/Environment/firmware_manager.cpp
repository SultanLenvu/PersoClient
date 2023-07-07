#include "firmware_manager.h"

FirmwareManager::FirmwareManager(QObject *parent, LogSystem *logger)
    : QObject(parent) {
  Logger = logger;

  Programmer = nullptr;
  ProgrammerThread = nullptr;

  processingFirmwarePath(DEFAULT_FIRMWARE_FILE_PATH);
}

FirmwareManager::~FirmwareManager() {
  delete Programmer;
  delete FirmwareFileInfo;
}

InterfaceProgrammer *FirmwareManager::programmer() const { return Programmer; }

void FirmwareManager::performErasing() {
  delete ProgrammerThread;
  ProgrammerThread = new QThread(this);

  // delete Programmer;
  Programmer = new JLinkManualProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  connect(Programmer, &InterfaceProgrammer::logging, Logger,
          &LogSystem::programmerLog);
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::erase);
  connect(Programmer, &InterfaceProgrammer::operationFinished, ProgrammerThread,
          &QThread::quit);
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);

  ProgrammerThread->start();

  //  while (ProgrammerThread->isFinished() == false)
  //    QApplication::processEvents();
}

void FirmwareManager::performLoading() {
  delete ProgrammerThread;
  ProgrammerThread = new QThread(this);

  // delete Programmer;
  Programmer = new JLinkManualProgrammer();
  Programmer->moveToThread(ProgrammerThread);

  connect(Programmer, &InterfaceProgrammer::logging, Logger,
          &LogSystem::programmerLog);
  connect(ProgrammerThread, &QThread::started, Programmer,
          &InterfaceProgrammer::erase);
  connect(Programmer, &InterfaceProgrammer::operationFinished, ProgrammerThread,
          &QThread::quit);
  connect(ProgrammerThread, &QThread::finished, Programmer,
          &QObject::deleteLater);

  ProgrammerThread->start();

  //  while (ProgrammerThread->isFinished() == false)
  //    QApplication::processEvents();

  //  Programmer->load(FirmwareFile);
}

void FirmwareManager::performAutoLoading() {}

void FirmwareManager::setFirmwareFile(const QString &path) {
  delete FirmwareFile;
  delete FirmwareFileInfo;

  processingFirmwarePath(path);

  if (FirmwareFile == nullptr)
    emit logging(QString("The firmware file is missing in the default "
                         "directory, select it manually."));
}

void FirmwareManager::processingFirmwarePath(const QString &path) {
  FirmwareFileInfo = new QFileInfo(path);

  if ((FirmwareFileInfo->exists()) && (FirmwareFileInfo->isFile()))
    FirmwareFile = new QFile(path, this);
  else
    FirmwareFile = nullptr;
}
