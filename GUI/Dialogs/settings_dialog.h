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

  // Настойки логгирования
  QGroupBox* LogSystemGroupBox;
  QGridLayout* LogSystemMainLayout;
  QLabel* LogSystemGlobalEnableLabel;
  QCheckBox* LogSystemGlobalEnableCheckBox;
  QLabel* LogSystemExtendedEnableLabel;
  QCheckBox* LogSystemExtendedEnableCheckBox;
  QLabel* LogSystemMessageMaxSizeLabel;
  QLineEdit* LogSystemMessageMaxSizeLineEdit;

  // Сеть
  QGroupBox* PersoServerGroupBox;
  QGridLayout* PersoServerMainLayout;
  QLabel* PersoServerIpAddressLabel;
  QLineEdit* PersoServerIpAddressLineEdit;
  QLabel* PersoServerPortLabel;
  QLineEdit* PersoServerPortLineEdit;

  // Программатор
  QGroupBox* ProgrammerGroupBox;
  QGridLayout* ProgrammerMainLayout;
  QLabel* ProgrammerExeFilePathLabel;
  QLineEdit* ProgrammerExeFilePathLineEdit;
  QPushButton* ProgrammerExeFilePathPushButton;
  QLabel* ProgrammerSpeedLabel;
  QLineEdit* ProgrammerSpeedLineEdit;

  // Принтер
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

  QSpacerItem* VS1;

  // Кнопки
  QHBoxLayout* ButtonLayout;
  QPushButton* ApplyPushButton;
  QPushButton* RejectPushButton;

 public:
  explicit SettingsDialog(QWidget* parent);
  ~SettingsDialog();

  virtual void accept(void) override;

 private:
  Q_DISABLE_COPY_MOVE(SettingsDialog)
  void create(void);
  bool check() const;
  void save(void);

  void createStickerPrinterProxyWidget(void);

 private slots:
  void programmerExeFilePathPushButton_slot(void);
  void stickerPrinterLibPathPushButton_slot(void);
  void stickerPrinterUseEthernetStateChanged_slot(int32_t state);

 signals:
  void applyNewSettings(void);
};

#endif  // SETTINGSINPUTDIALOG_H
