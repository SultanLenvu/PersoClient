#ifndef JLINKMANUALPROGRAMMER_H
#define JLINKMANUALPROGRAMMER_H

#include "interface_programmer.h"

class JLinkManualProgrammer : public InterfaceProgrammer {
public:
  explicit JLinkManualProgrammer(QObject *parent);

  virtual void loadFirmware(void) override;
  virtual void eraseFirmware(void) override;
};

#endif // JLINKMANUALPROGRAMMER_H
