#include "abstract_client_command.h"

#include <QDataStream>
#include <QIODevice>
#include <QJsonDocument>

AbstractClientCommand::AbstractClientCommand(const QString& name)
    : QObject{nullptr} {
  setObjectName(name);
}

AbstractClientCommand::~AbstractClientCommand() {}

AbstractClientCommand::AbstractClientCommand() {}

void AbstractClientCommand::sendLog(const QString& log) {
  emit logging("Command " + objectName() + " - " + log);
}

void AbstractClientCommand::generateDataBlock(QByteArray& dataBlock) {
  QJsonDocument requestDocument(Request);

  // Инициализируем блок данных и сериализатор
  dataBlock.clear();
  QDataStream serializator(&dataBlock, QIODevice::WriteOnly);
  serializator.setVersion(QDataStream::Qt_5_12);

  // Формируем единый блок данных для отправки
  serializator << uint32_t(0) << requestDocument.toJson();
  serializator.device()->seek(0);
  serializator << uint32_t(dataBlock.size() - sizeof(uint32_t));
}

bool AbstractClientCommand::processDataBlock(const QByteArray& dataBlock) {
  QJsonParseError status;
  QJsonDocument responseDocument = QJsonDocument::fromJson(dataBlock, &status);

  // Если пришел некорректный JSON
  if (status.error != QJsonParseError::NoError) {
    return false;
  }

  // Выделяем список пар ключ-значение из JSON-файла
  Response = responseDocument.object();
  return true;
}
