#ifndef SERVERCONNECTIONGUISUBKERNEL_H
#define SERVERCONNECTIONGUISUBKERNEL_H

#include <QPlainTextEdit>

#include "abstract_gui_subkernel.h"
#include "types.h"

class ServerConnectionGuiSubkernel final : public AbstractGuiSubkernel {
  Q_OBJECT

 public:
  explicit ServerConnectionGuiSubkernel() = default;
  ~ServerConnectionGuiSubkernel() = default;
  Q_DISABLE_COPY_MOVE(ServerConnectionGuiSubkernel)

 public slots:
  void connect(void);
  void disconnect(void);
  void echo(void);

  void launchProductionLine(void);
  void shutdownProductionLine(void);
  void getProductionLineData(void);

  void requestBox(void);
  void getCurrentBoxData(void);
  void refundCurrentBox(void);
  void completeCurrentBox(void);

  void releaseTransponder(void);
  void confirmTransponderRelease(void);
  void rereleaseTransponder(void);
  void confirmTransponderRerelease(void);
  void rollbackTransponder(void);
  void getCurrentTransponderData(void);
  void getTransponderData(void);

  void printBoxSticker(void);
  void printLastBoxSticker(void);
  void printPalletSticker(void);
  void printLastPalletSticker(void);

  void generateDisconnectionAlert(void);

 signals:
  void connect_signal(void);
  void disconnect_signal(void);
  void echo_signal(void);

  void launchProductionLine_signal(const StringDictionary& param);
  void shutdownProductionLine_signal(void);
  void getProductionLineData_signal(void);

  void requestBox_signal(void);
  void getCurrentBoxData_signal(void);
  void refundCurrentBox_signal(void);
  void completeCurrentBox_signal(void);

  void releaseTransponder_signal(void);
  void confirmTransponderRelease_signal(const StringDictionary& param);
  void rereleaseTransponder_signal(const StringDictionary& param);
  void confirmTransponderRerelease_signal(const StringDictionary& param);
  void rollbackTransponder_signal(void);
  void getCurrentTransponderData_signal(void);
  void getTransponderData_signal(const StringDictionary& param);

  void printBoxSticker_signal(const StringDictionary& data);
  void printLastBoxSticker_signal(void);
  void printPalletSticker_signal(const StringDictionary& data);
  void printLastPalletSticker_signal(void);
};

#endif  // SERVERCONNECTIONGUISUBKERNEL_H
