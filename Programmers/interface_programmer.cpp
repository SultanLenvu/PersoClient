#include "interface_programmer.h"

IProgrammer::IProgrammer(QObject* parent, ProgrammerType type)
    : QObject(parent) {
  setObjectName("IProgrammer");
  Type = type;

  // Регистрация в мета-объектной системе Qt пользовательские типы
  qRegisterMetaType<ExecutionStatus>("ExecutionStatus");
}

IProgrammer::~IProgrammer() {}

IProgrammer::ProgrammerType IProgrammer::type() const {
  return Type;
}

bool IProgrammer::checkFirmwareFile(const QFile* firmware) {
  if (!firmware) {
    return false;
  }

  QFileInfo info(*firmware);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проврека на размер
  if (info.size() != FIRMWARE_SIZE)
    return false;

  return true;
}

bool IProgrammer::checkDataFile(const QFile* data) {
  if (!data) {
    return false;
  }

  QFileInfo info(*data);

  // Проверка на существование
  if ((!info.exists()) || (!info.isFile()))
    return false;

  // Проверка на размер
  if ((info.size() > USER_DATA_FLASH_SIZE) || (info.size() == 0))
    return false;

  return true;
}
