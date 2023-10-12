#ifndef TE310PRINTER_H
#define TE310PRINTER_H

#include "isticker_printer.h"

class TE310Printer : public IStickerPrinter {
  Q_OBJECT

 private:
  // Прототипы библиотечных функции
  typedef int (*TscAbout)(void);
  typedef int (*TscOpenPort)(const char*);
  typedef int (*TscSendCommand)(const char*);
  typedef int (*TscClosePort)(void);

 private:
  bool LogEnable;
  QString TscLibPath;
  QLibrary* TscLib;

  QMap<QString, QString> LastTransponderSticker;

  // Библиотечные функции
  TscAbout about;
  TscOpenPort openPort;
  TscSendCommand sendCommand;
  TscClosePort closePort;

 public:
  explicit TE310Printer(QObject* parent);

  virtual ReturnStatus printTransponderSticker(
      const QMap<QString, QString>* parameters) override;
  virtual ReturnStatus printLastTransponderSticker(void) override;
  virtual ReturnStatus exec(const QStringList* commandScript) override;

  virtual void applySetting(void) override;

 private:
  void loadSetting(void);
  void loadTscLib(void);

  void printNkdSticker(const QMap<QString, QString>* parameters);
  void printZsdSticker(const QMap<QString, QString>* parameters);
};

#endif // TE310PRINTER_H
