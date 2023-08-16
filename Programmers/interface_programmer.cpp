#include "interface_programmer.h"

InterfaceProgrammer::InterfaceProgrammer(QObject *parent, ProgrammerType type)
    : QObject(parent) {
  Type = type;
}

InterfaceProgrammer::~InterfaceProgrammer() {}

InterfaceProgrammer::ProgrammerType InterfaceProgrammer::type() const {
  return Type;
}

bool InterfaceProgrammer::checkFileName(const QString& name) {
  QFileInfo info(name);

  if ((!info.exists()) || (!info.isFile()))
    return false;
  else
    return true;
}
