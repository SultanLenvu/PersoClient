#ifndef MASTER_USER_INTERFACE_H
#define MASTER_USER_INTERFACE_H

#include <QtWidgets>
#include "types.h"

#include "log_display_widget.h"
#include "production_manager_widget.h"
#include "programmator_widget.h"
#include "sticker_printer_widget.h"

class MasterUserInterface final : public QWidget {
 private:
  QHBoxLayout* MainLayout;
  QTabWidget* Tabs;

  LogDisplayWidget* LogDisplay;
  ProductionManagerWidget* ProductionManagerTab;
  ProgrammatorWidget* ProgrammatorTab;
  StickerPrinterWidget* StickerPrinterTab;

 public:
  explicit MasterUserInterface(QWidget* parent = nullptr);
  ~MasterUserInterface() = default;
  Q_DISABLE_COPY_MOVE(MasterUserInterface);

 public:
  void displayLog(const QString& log);
  void clearLog();

 private:
  void createTabs();
  void createStickerPrinter();
  void createProgrammator();
  void createProductionManager();
  void createLogDisplay();

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

  void logOn_trigger(const StringDictionary& param);
  void logOut_trigger();

  //  void requestBox_trigger();
  void refundBox_trigger();
  void completeBox_trigger();

  void releaseTransponder_trigger();
  void rereleaseTransponder_trigger(const StringDictionary& param);
  //  void rollbackTransponder_trigger();

  void printLastTransponderSticker_trigger();
  void printTransponderSticker_trigger(const StringDictionary& param);
  void execCommandScript_trigger(const QStringList& script);

  void programMemory_trigger(const QString& path);
  void readMemory_trigger();
  void eraseMemory_trigger();

  void readUserData_trigger();
  void programUserData_trigger(const QString& path);
  void readUcid_trigger();

  void unlockMemory_trigger();
  void lockMemory_trigger();
};

#endif  // MASTER_USER_INTERFACE_H
