#ifndef PROGRAMMERCCONNECTOR_H
#define PROGRAMMERCCONNECTOR_H

#include "i_object_connector.h"

template <typename F, typename S>
class ProgrammerConnector : public IObjectConnector<F, S> {
 public:
  explicit ProgrammerConnector() = default;
  virtual ~ProgrammerConnector() = default;
  Q_DISABLE_COPY_MOVE(ProgrammerConnector)

  // IObjectConnector interface
 public:
  virtual void plug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::programMemory_signal, sobj, &S::programMemory);
    QObject::connect(fobj, &F::readMemory_signal, sobj, &S::readMemory);
    QObject::connect(fobj, &F::eraseMemory_signal, sobj, &S::eraseMemory);

    QObject::connect(fobj, &F::programUserData_signal, sobj,
                     &S::programUserData);
    QObject::connect(fobj, &F::readUserData_signal, sobj, &S::readUserData);
    QObject::connect(fobj, &F::readUcid_signal, sobj, &S::readTransponderUcid);

    QObject::connect(fobj, &F::unlockMemory_signal, sobj, &S::unlockMemory);
    QObject::connect(fobj, &F::lockMemory_signal, sobj, &S::lockMemory);

    QObject::connect(sobj, &S::transponderUcidReady, fobj, &F::displayUcid);
  }
  virtual void unplug(const F* fobj, const S* sobj) override {
    QObject::disconnect(fobj, &F::programMemory_signal, sobj,
                        &S::programMemory);
    QObject::disconnect(fobj, &F::readMemory_signal, sobj, &S::readMemory);
    QObject::disconnect(fobj, &F::eraseMemory_signal, sobj, &S::eraseMemory);

    QObject::disconnect(fobj, &F::programUserData_signal, sobj,
                        &S::programUserData);
    QObject::disconnect(fobj, &F::readUserData_signal, sobj, &S::readUserData);
    QObject::disconnect(fobj, &F::readUcid_signal, sobj,
                        &S::readTransponderUcid);

    QObject::disconnect(fobj, &F::unlockMemory_signal, sobj, &S::unlockMemory);
    QObject::disconnect(fobj, &F::lockMemory_signal, sobj, &S::lockMemory);

    QObject::disconnect(sobj, &S::transponderUcidReady, fobj, &F::displayUcid);
  }
};

#endif  // PROGRAMMERCCONNECTOR_H
