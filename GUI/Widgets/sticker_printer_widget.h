#ifndef STICKERPRINTERUSERINTERFACE_H
#define STICKERPRINTERUSERINTERFACE_H

#include <QtWidgets>

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

 private:
  Q_DISABLE_COPY_MOVE(StickerPrinterWidget)
  void create(void);
  void connectDependencies(void);

 private slots:
  void execCommandSript(void);

 signals:
  void execCommandScript_signal(const QStringList& script);
};

#endif // STICKERPRINTERUSERINTERFACE_H
