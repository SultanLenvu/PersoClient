#include "interface_programmer.h"

InterfaceProgrammer::InterfaceProgrammer(QObject *parent, ProgrammerType type)
    : QObject(parent) {
  Type = type;

  LoadingFirmware = nullptr;
  LoadingUserData = nullptr;
}

InterfaceProgrammer::~InterfaceProgrammer() {}

InterfaceProgrammer::ProgrammerType InterfaceProgrammer::type() const {
  return Type;
}

void InterfaceProgrammer::setLoadingFirmware(QFile *firmware) {
  LoadingFirmware = firmware;
}

void InterfaceProgrammer::setLoadingUserData(QFile *userData) {
  LoadingUserData = userData;
}
