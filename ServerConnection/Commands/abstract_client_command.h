#ifndef ABSTRACTCLIENTCOMMAND_H
#define ABSTRACTCLIENTCOMMAND_H

#include <QJsonObject>
#include <QObject>

#include <General/types.h>

class AbstractClientCommand : public QObject {
  Q_OBJECT

 protected:
  QJsonObject Request;
  QJsonObject Response;

 public:
  explicit AbstractClientCommand(const QString& name);
  virtual ~AbstractClientCommand();

  virtual const QString& name(void) = 0;
  virtual ReturnStatus generate(QByteArray& dataBlock) = 0;
  virtual ReturnStatus generate(const StringDictionary& param,
                                QByteArray& dataBlock) = 0;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock) = 0;
  virtual ReturnStatus processResponse(const QByteArray& dataBlock,
                                       StringDictionary& responseData) = 0;
  virtual void clear(void) = 0;

 private:
  AbstractClientCommand();
  Q_DISABLE_COPY_MOVE(AbstractClientCommand)

 protected:
  void sendLog(const QString& log);
  void generateDataBlock(QByteArray& dataBlock);
  bool processDataBlock(const QByteArray& dataBlock);

 signals:
  void logging(const QString& log);
};

#endif  // ABSTRACTCLIENTCOMMAND_H
