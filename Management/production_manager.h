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

  std::shared_ptr<StringDictionary> TransponderData;
  std::shared_ptr<StringDictionary> PreviousTransponderData;

 public:
  explicit ProductionManager(const QString& name);
  ~ProductionManager();

 public slots:
  virtual void applySettings(void) override;

  void initServerConnection(const std::shared_ptr<StringDictionary> param);
  void resetServerConnection(void);
  void echoServer(void);

  void releaseTransponder(void);
  void rereleaseTransponder(const std::shared_ptr<StringDictionary> param);
  void rollbackProductionLine(void);

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
  void displayTransponderData_signal(
      const std::shared_ptr<StringDictionary> data);
  void authorizationCompleted(void);
};

#endif  // ProductionManager_H
