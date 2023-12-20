#include "programmer_manager.h"

ProgrammerManager::ProgrammerManager(const QString& name)
    : AbstractManager{name} {}

ProgrammerManager::~ProgrammerManager() {}

void ProgrammerManager::programMemory(const std::shared_ptr<QString> path) {}

void ProgrammerManager::readMemory() {}

void ProgrammerManager::eraseMemory() {}

void ProgrammerManager::programUserData(const std::shared_ptr<QString> path) {}

void ProgrammerManager::readUserData() {}

void ProgrammerManager::readUcid() {}

void ProgrammerManager::unlockMemory() {}

void ProgrammerManager::lockMemory() {}
