#ifndef ProductionManager_H
#define ProductionManager_H

#include <QFile>

#include "abstract_manager.h"
#include "abstract_programmer.h"
#include "abstract_server_connection.h"
#include "abstract_sticker_printer.h"

class ProductionManager : public AbstractManager {
  Q_OBJECT

 private:
  std::unique_ptr<AbstractServerConnection> Server;
  std::unique_ptr<AbstractProgrammer> Programmer;
  std::unique_ptr<AbstractStickerPrinter> StickerPrinter;

  std::unique_ptr<QFile> Firmware;

  StringDictionary BoxData;
  StringDictionary TransponderData;

 public:
  explicit ProductionManager(const QString& name);
  ~ProductionManager();

  // AbstractManager interface
 public:
  virtual void onInstanceThreadStarted(void) override;
  virtual Type type() const override;

 public slots:
  virtual void applySettings(void) override;

  void connectToServer(void);
  void disconnectFromServer(void);
  void initServerConnection(const std::shared_ptr<StringDictionary> param);
  void resetServerConnection(void);
  void echoServer(void);

  void requestBox(void);
  void getCurrentBoxData(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void rereleaseTransponder(const std::shared_ptr<StringDictionary> param);
  void rollbackTransponder(void);
  void getCurrentTransponderData(void);
  void getTransponderData(const std::shared_ptr<StringDictionary> param);

  void printBoxSticker(const std::shared_ptr<StringDictionary> param);
  void printLastBoxSticker(void);
  void printPalletSticker(const std::shared_ptr<StringDictionary> param);
  void printLastPalletSticker(void);

 private:
  Q_DISABLE_COPY_MOVE(ProductionManager)
  void loadSettings(void);
  void sendLog(const QString& log);

  void createProgrammer(void);
  void createServer(void);
  void createStickerPrinter(void);

 signals:
  void displayTransponderData_signal(const StringDictionary& data);
  void displayBoxData_signal(const StringDictionary& data);
  void authorizationCompleted(void);
};

#endif  // ProductionManager_H
