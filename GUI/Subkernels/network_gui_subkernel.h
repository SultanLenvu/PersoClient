#ifndef NETWORKGUISUBKERNEL_H
#define NETWORKGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class NetworkGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 private:
  std::shared_ptr<AbstractGui> CurrentGui;
  std::shared_ptr<AbstractManager> Manager;

 public:
  explicit NetworkGuiSubkernel(const QString& name);
  ~NetworkGuiSubkernel();

  // AbstractGuiSubkernel interface
 public:
  virtual void setCurrentGui(std::shared_ptr<AbstractGui> gui) override;
  virtual void setManager(std::shared_ptr<AbstractManager> manager) override;

 private:
  Q_DISABLE_COPY_MOVE(NetworkGuiSubkernel);

  void connectProductionGui(void);
  void connectTestingGui(void);
  void connectMasterGui(void);

  void connectPushButton_slot(void);
  void disconnectButton_slot(void);
  void echoRequestButton_slot(void);
  void authorizePushButton_slot(void);
  void loadTransponderFirmwareButton_slot(void);
  void reloadTransponderFirmwareButton_slot(void);
  void rollbackProductionLinePushButton_slot(void);
  void printBoxStickerButton_slot(void);
  void printPalletStickerButton_slot(void);

 signals:
  // Сигналы для менеджера
  void connectToServer_signal(void);
  void disconnectFromServer_signal(void);

  void echoServer_signal(void);
  void authorize_signal(const QSharedPointer<QHash<QString, QString>> data);

  void loadFirmware_signal();
  void reloadFirmware_signal(const QString& pan);
  void rollback_signal(void);

  void printBoxSticker_signal(
      const QSharedPointer<QHash<QString, QString>> data);
  void printLastBoxSticker_signal(void);
  void printPalletSticker_signal(
      const QSharedPointer<QHash<QString, QString>> data);
  void printLastPalletSticker_signal(void);
};

#endif  // NETWORKGUISUBKERNEL_H
