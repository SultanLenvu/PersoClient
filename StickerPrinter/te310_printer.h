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
  QString TscLibPath;
  QLibrary* TscLib;

  // Библиотечные функции
  TscAbout about;
  TscOpenPort openPort;
  TscSendCommand sendCommand;
  TscClosePort closePort;

 public:
  explicit TE310Printer(QObject* parent);

  virtual void printTransponderSticker(
      const QMap<QString, QString>* parameters) override;
  virtual void applySetting(void) override;

 private:
  void printNkdSticker(void);
  void printZsdSticker(void);
  void loadSetting(void);
  void loadTscLib(void);
};

#endif // TE310PRINTER_H
