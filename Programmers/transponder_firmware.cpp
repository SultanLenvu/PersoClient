#include "transponder_firmware.h"

#include <QFileInfo>
#include "definitions.h"

TransponderFirmware::TransponderFirmware() {
  Data.reserve(TRANSPONDER_FIRMWARE_SIZE);
}

bool TransponderFirmware::setData(QByteArray& data) {
  if (data.size() != TRANSPONDER_FIRMWARE_SIZE) {
    return false;
  }

  Data = data;

  return true;
}

size_t TransponderFirmware::size() const {
  return Data.size();
}

bool TransponderFirmware::writeToFile(const QString& fileName) const {
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

bool TransponderFirmware::readFromFile(const QString& fileName) {
  QFile file(fileName);

  if (!checkFile(file)) {
    return false;
  }

  if (!file.open(QIODevice::ReadOnly)) {
    return false;
  }

  Data = file.readAll();

  return true;
}

bool TransponderFirmware::checkFile(const QFile& file) const {
  QFileInfo info(file);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проврека на размер
  if (info.size() != TRANSPONDER_FIRMWARE_SIZE)
    return false;

  return true;
}
