#include "network_manager.h"

NetworkManager::NetworkManager(const QString& name) : AbstractManager{name} {}

NetworkManager::~NetworkManager() {}

void NetworkManager::connectToServer() {}

void NetworkManager::disconnectFromServer() {}

void NetworkManager::echoServer() {}

void NetworkManager::authorize(const std::shared_ptr<StringDictionary> param) {}

void NetworkManager::releaseTransponder() {}

void NetworkManager::rereleaseTransponder(
    const std::shared_ptr<StringDictionary> param) {}

void NetworkManager::rollbackProductionLine() {}

void NetworkManager::printBoxSticker(
    const std::shared_ptr<StringDictionary> data) {}

void NetworkManager::printLastBoxSticker() {}

void NetworkManager::printPalletSticker(
    const std::shared_ptr<StringDictionary> data) {}

void NetworkManager::printLastPalletSticker() {}
