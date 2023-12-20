#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "abstract_manager.h"

class NetworkManager : public AbstractManager {
  Q_OBJECT
 public:
  explicit NetworkManager(const QString& name);
  ~NetworkManager();

 public slots:
  void connectToServer(void);
  void disconnectFromServer(void);
  void echoServer(void);
  void authorize(const std::shared_ptr<StringDictionary> param);

  void releaseTransponder(void);
  void rereleaseTransponder(const std::shared_ptr<StringDictionary> param);
  void rollbackProductionLine(void);

  void printBoxSticker(const std::shared_ptr<StringDictionary> data);
  void printLastBoxSticker(void);
  void printPalletSticker(const std::shared_ptr<StringDictionary> data);
  void printLastPalletSticker(void);

 private:
  Q_DISABLE_COPY_MOVE(NetworkManager)

 signals:
  void displayTransponderData(const std::shared_ptr<StringDictionary> data);
  void authorizationCompleted(void);
};

#endif  // NETWORKMANAGER_H
