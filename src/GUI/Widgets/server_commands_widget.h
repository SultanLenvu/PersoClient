#ifndef SERVERCOMMANDSWIDGET_H
#define SERVERCOMMANDSWIDGET_H

#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "types.h"

class ServerCommandsWidget final : public QGroupBox {
  Q_OBJECT

 private:
  typedef void (ServerCommandsWidget::*MethodPtr)();
  QHash<QString, MethodPtr> CommandMethods;

 private:
  QVBoxLayout* Layout;
  QComboBox* ComboBox;
  QPushButton* Button;

 public:
  explicit ServerCommandsWidget(QWidget* parent = nullptr);
  ~ServerCommandsWidget() = default;
  Q_DISABLE_COPY_MOVE(ServerCommandsWidget)

 private:
  void create();
  void connectInternals();
  void createCommandMethod();

 private slots:
  void prepareCommandExecution();

  void connect();
  void disconnect();
  void echo();

  void launchProductionLine();
  void shutdownProductionLine();
  void getProductionLineData();

  void requestBox();
  void getCurrentBoxData();
  void refundCurrentBox();
  void completeCurrentBox();

  void getTransponderFirmware();
  void confirmTransponderFirmware();
  void repeatTransponderFirmware();
  void confirmRepeatedTransponderFirmware();
  void rollbackTransponder();
  void getCurrentTransponderData();
  void getTransponderData();

  void printBoxSticker();
  void printLastBoxSticker();
  void printPalletSticker();
  void printLastPalletSticker();

 signals:
  void connect_trigger();
  void disconnect_trigger();
  void echo_trigger();

  void launchProductionLine_trigger(const StringDictionary& param);
  void shutdownProductionLine_trigger();
  void getProductionLineData_trigger();

  void requestBox_trigger();
  void getCurrentBoxData_trigger();
  void refundCurrentBox_trigger();
  void completeCurrentBox_trigger();

  void getTransponderFirmware_trigger();
  void confirmTransponderFirmware_trigger(const StringDictionary& param);
  void repeatTransponderFirmware_trigger(const StringDictionary& param);
  void confirmRepeatedTransponderFirmware_trigger(
      const StringDictionary& param);
  void rollbackTransponder_trigger();
  void getCurrentTransponderData_trigger();
  void getTransponderData_trigger(const StringDictionary& param);

  void printBoxSticker_trigger(const StringDictionary& data);
  void printLastBoxSticker_trigger();
  void printPalletSticker_trigger(const StringDictionary& data);
  void printLastPalletSticker_trigger();
};

#endif  // SERVERCOMMANDSWIDGET_H
