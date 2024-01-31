#ifndef TE310PRINTER_H
#define TE310PRINTER_H

#include <QHostAddress>
#include <QHostInfo>

#include "abstract_sticker_printer.h"

class TE310Printer : public AbstractStickerPrinter {
  Q_OBJECT

 private:
  // Прототипы библиотечных функции
  typedef int (*TscAbout)(void);
  typedef int (*TscOpenPort)(const char*);
  typedef int (*TscSendCommand)(const char*);
  typedef int (*TscClosePort)(void);
#ifdef __linux__
  typedef int (*TscOpenEthernet)(const char*, int);
#endif /* __linux__ */

 private:
#ifdef __linux__
  QHostAddress IPAddress;
  int Port;
#endif /* __linux__ */

  QString TscLibPath;
  std::unique_ptr<QLibrary> TscLib;

  static StringDictionary LastTransponderSticker;
  static StringDictionary LastBoxSticker;
  static StringDictionary LastPalletSticker;

  // Библиотечные функции
  TscAbout about;
  TscOpenPort openPort;
  TscSendCommand sendCommand;
  TscClosePort closePort;
#ifdef __linux__
  TscOpenEthernet openEthernet;
#endif /* __linux__ */

 public:
  explicit TE310Printer(const QString& name);
#ifdef __linux__
  explicit TE310Printer(QObject* parent, const QHostAddress& ip, int port);
#endif /* __linux__ */

  virtual ReturnStatus checkConfig(void) override;
  virtual StickerPrinterType type(void) override;

  virtual ReturnStatus printTransponderSticker(
      const StringDictionary& param) override;
  virtual ReturnStatus printLastTransponderSticker(void) override;

  virtual ReturnStatus printBoxSticker(const StringDictionary& param) override;
  virtual ReturnStatus printLastBoxSticker(void) override;

  virtual ReturnStatus printPalletSticker(
      const StringDictionary& param) override;
  virtual ReturnStatus printLastPalletSticker(void) override;

  virtual ReturnStatus exec(const QStringList& commandScript) override;

  virtual void applySetting(void) override;

 private:
  Q_DISABLE_COPY_MOVE(TE310Printer);
  void loadSetting(void);
  void sendLog(const QString& log);

  void loadTscLib(void);

  bool initConnection(void);

  void printNkdSticker(const StringDictionary& param);
  void printZsdSticker(const StringDictionary& param);
};

#endif  // TE310PRINTER_H