#ifndef STICKERPRINTERUSERINTERFACE_H
#define STICKERPRINTERUSERINTERFACE_H

#include <QtWidgets>

#include "types.h"

class StickerPrinterWidget : public QWidget {
  Q_OBJECT
 private:
  QHBoxLayout* MainLayout;

  QGroupBox* ControlPanel;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintTransponderStickerButton;
  QSpacerItem* ControlPanelVS1;
  QPushButton* ExecCommandScriptButton;

  QGroupBox* CommandSriptGroup;
  QVBoxLayout* CommandSriptLayout;
  QPlainTextEdit* CommandSriptInput;

 public:
  explicit StickerPrinterWidget(QWidget* parent = nullptr);
  ~StickerPrinterWidget() = default;
  Q_DISABLE_COPY_MOVE(StickerPrinterWidget)

 private:
  void create();
  void connectInternals();

 private slots:
  void execCommandSript();
  void printTransponderSticker();

 signals:
  void printLastTransponderSticker_trigger();
  void printTransponderSticker_trigger(const StringDictionary& param);
  void execCommandScript_trigger(const QStringList& script);
};

#endif // STICKERPRINTERUSERINTERFACE_H
