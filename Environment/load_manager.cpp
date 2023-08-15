#include "load_manager.h"

LoadManager::LoadManager(QObject *parent) : QObject(parent) {
  Programmer = new JLinkManualProgrammer(this);
}

InterfaceProgrammer *LoadManager::programmer() const { return Programmer; }

void LoadManager::processingFirmwareLoading() { Programmer->loadFirmware(); }

void LoadManager::processingFirmwareErasing() { Programmer->eraseFirmware(); }
