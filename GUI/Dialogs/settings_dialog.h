#ifndef SETTINGSINPUTDIALOG_H
#define SETTINGSINPUTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QHostAddress>
#include <QtWidgets>

class SettingsDialog : public QDialog {
  Q_OBJECT
 private:
  QSettings Settings;
  QSize DesktopGeometry;

  QVBoxLayout* MainLayout;

 private:  // Настойки логгирования
  QGroupBox* LogSystemGroupBox;
  QGridLayout* LogSystemMainLayout;
  QLabel* LogSystemGlobalEnableLabel;
  QCheckBox* LogSystemGlobalEnableCheckBox;
  QLabel* LogSystemMessageMaxSizeLabel;
  QLineEdit* LogSystemMessageMaxSizeLineEdit;
  QLabel* LogSystemFileDirLabel;
  QLineEdit* LogSystemFileDirLineEdit;
  QPushButton* LogSystemFileDirPushButton;
  QLabel* LogSystemFileMaxNumberLabel;
  QLineEdit* LogSystemFileMaxNumberLineEdit;

 private:  // Сеть
  QGroupBox* PersoServerGroupBox;
  QGridLayout* PersoServerMainLayout;
  QLabel* PersoServerIpAddressLabel;
  QLineEdit* PersoServerIpAddressLineEdit;
  QLabel* PersoServerPortLabel;
  QLineEdit* PersoServerPortLineEdit;

 private:  // Программатор
  QGroupBox* ProgrammerGroupBox;
  QGridLayout* ProgrammerMainLayout;
  QLabel* ProgrammerExeFilePathLabel;
  QLineEdit* ProgrammerExeFilePathLineEdit;
  QPushButton* ProgrammerExeFilePathPushButton;
  QLabel* ProgrammerSpeedLabel;
  QLineEdit* ProgrammerSpeedLineEdit;

 private:  // Принтер
  QGroupBox* StickerPrinterGroupBox;
  QGridLayout* StickerPrinterMainLayout;
  QLabel* StickerPrinterLibPathLabel;
  QLineEdit* StickerPrinterLibPathLineEdit;
  QPushButton* StickerPrinterLibPathPushButton;
  QLabel* StickerPrinterNameLabel;
  QLineEdit* StickerPrinterNameLineEdit;
  QLabel* StickerPrinterUseEthernetLabel;
  QCheckBox* StickerPrinterUseEthernetCheck;

  QWidget* StickerPrinterProxyWidget;
  QGridLayout* StickerPrinterProxyWidgetLayout;
  QLabel* StickerPrinterIpLabel;
  QLineEdit* StickerPrinterIpLineEdit;
  QLabel* StickerPrinterPortLabel;
  QLineEdit* StickerPrinterPortLineEdit;

 private:
  QSpacerItem* VS1;

 private:  // Кнопки
  QHBoxLayout* ButtonLayout;
  QPushButton* ApplyPushButton;
  QPushButton* RejectPushButton;

 public:
  explicit SettingsDialog(QWidget* parent);
  ~SettingsDialog();

 public:  // QDialog interface
  virtual void accept(void) override;

 private:
  Q_DISABLE_COPY_MOVE(SettingsDialog)
  void create(void);

  void createLogSystem(void);
  void createPersoServer(void);
  void createProgrammator(void);
  void createStickerPrinter(void);
  void createStickerPrinterProxyWidget(void);
  void createButtons(void);

  bool check() const;
  void save(void);

 private slots:
  void logSystemFileDirPushButton_slot(void);
  void programmerExeFilePathPushButton_slot(void);
  void stickerPrinterLibPathPushButton_slot(void);
  void stickerPrinterUseEthernetStateChanged_slot(int32_t state);

 signals:
  void applyNewSettings(void);
};

#endif  // SETTINGSINPUTDIALOG_H
