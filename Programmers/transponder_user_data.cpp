#include "transponder_user_data.h"
#include "definitions.h"

#include <QFileInfo>

bool TransponderUserData::setData(QByteArray& data) {
  if (data.size() != TRANSPONDER_USER_DATA_SIZE) {
    return false;
  }

  Data = data;

  return true;
}

bool TransponderUserData::valid() const {
  return !Data.isEmpty();
}

bool TransponderUserData::writeToFile(const QString& fileName) const {
  QFile file(fileName);
  if (file.open(QIODevice::WriteOnly)) {
    return false;
  }

  qint64 bytesWritten = file.write(Data);
  file.close();

  if (bytesWritten == -1) {
    return false;
  }

  return true;
}

bool TransponderUserData::readFromFile(QFile &file)
{
  if (!checkFile(file)) {
    return false;
  }

  if (!file.open(QIODevice::ReadOnly)) {
    return false;
  }

  Data.reserve(TRANSPONDER_USER_DATA_SIZE);
  Data = file.readAll();

  return true;
}

bool TransponderUserData::readFromFile(const QString &fileName)
{
  QFile file(fileName);
  return readFromFile(file);
}

bool TransponderUserData::checkFile(const QFile& file) const {
  QFileInfo info(file);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проверка на размер
  if ((info.size() > TRANSPONDER_USER_DATA_SIZE) || (info.size() == 0))
    return false;

  return true;
}
