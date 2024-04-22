#ifndef PERSOSERVERMANAGER_H
#define PERSOSERVERMANAGER_H

#include <QFile>

#include "i_server_connection.h"
#include "production_unit_context.h"
#include "progressable_async_wrapper.h"

class AsyncServerConnection final : public AbstractAsyncObject {
  Q_OBJECT
 private:
  std::shared_ptr<ProductionUnitContext> Context;
  std::shared_ptr<IServerConnection> Server;

 public:
  explicit AsyncServerConnection(const QString& name,
                                 std::shared_ptr<ProductionUnitContext> context,
                                 std::shared_ptr<IServerConnection> server);
  ~AsyncServerConnection() = default;
  Q_DISABLE_COPY_MOVE(AsyncServerConnection)

 public slots:
  void connect(void);
  void disconnect(void);

  void echo(void);

  void launchProductionLine(const StringDictionary& param);
  void shutdownProductionLine(void);
  void getProductionLineData(void);

  void requestBox(void);
  void getCurrentBoxData(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void confirmTransponderRelease(const StringDictionary& param);
  void rereleaseTransponder(const StringDictionary& param);
  void confirmTransponderRerelease(
      const StringDictionary& param);
  void rollbackTransponder(void);
  void getCurrentTransponderData(void);
  void getTransponderData(const StringDictionary& param);

  void printBoxSticker(const StringDictionary& param);
  void printLastBoxSticker(void);
  void printPalletSticker(const StringDictionary& param);
  void printLastPalletSticker(void);
};

#endif  // PERSOSERVERMANAGER_H
