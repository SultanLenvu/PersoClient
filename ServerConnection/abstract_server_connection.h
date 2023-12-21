#ifndef ABSTARCTPERSOCLIENT_H
#define ABSTARCTPERSOCLIENT_H

#include <QObject>

#include "types.h"

class AbstractServerConnection : public QObject {
  Q_OBJECT
 public:
  explicit AbstractServerConnection(const QString& name);
  virtual ~AbstractServerConnection();

  virtual ReturnStatus connect(void) = 0;
  virtual void disconnect(void) = 0;

  virtual ReturnStatus echo(void);

  virtual ReturnStatus logIn(const StringDictionary& param) = 0;
  virtual ReturnStatus logOut(void) = 0;

  virtual ReturnStatus update(StringDictionary& result) = 0;
  virtual ReturnStatus getTransponderData(const StringDictionary& param,
                                          StringDictionary& result) = 0;

  virtual ReturnStatus release(StringDictionary& result) = 0;
  virtual ReturnStatus confirmRelease(const StringDictionary& param) = 0;
  virtual ReturnStatus rerelease(const StringDictionary& param,
                                 StringDictionary& result) = 0;
  virtual ReturnStatus confirmRerelease(const StringDictionary& param);
  virtual ReturnStatus rollback(void) = 0;

  virtual ReturnStatus printBoxSticker(const StringDictionary& param) = 0;
  virtual ReturnStatus printLastBoxSticker(void) = 0;
  virtual ReturnStatus printPalletSticker(const StringDictionary& param) = 0;
  virtual ReturnStatus printLastPalletSticker(void) = 0;

  virtual void applySettings(void) = 0;

 private:
  AbstractServerConnection();
  Q_DISABLE_COPY_MOVE(AbstractServerConnection)

 signals:
  void logging(const QString& log);
  void disconnected(void);
};

#endif  // ABSTARCTPERSOCLIENT_H
