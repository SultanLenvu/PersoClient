#ifndef PRODUCTIONCONTEXTCONNECTOR_H
#define PRODUCTIONCONTEXTCONNECTOR_H

#include "i_object_connector.h"

template <typename F, typename S>
class ProductionContextConnector : public IObjectConnector<F, S> {
 public:
  explicit ProductionContextConnector() = default;
  virtual ~ProductionContextConnector() = default;
  Q_DISABLE_COPY_MOVE(ProductionContextConnector)

  // IObjectConnector interface
 public:
  virtual void plug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::stateChanged, sobj, &S::displayStateData);
    QObject::connect(fobj, &F::boxChanged, sobj, &S::displayBoxData);
    QObject::connect(fobj, &F::transponderChanged, sobj,
                     &S::displayTransponderData);
    //    QObject::connect(fobj, &F::firmwareChanged, sobj,
    //    &S::displayFirmware);
  }
  virtual void unplug(const F* fobj, const S* sobj) override {
    QObject::disconnect(fobj, &F::stateChanged, sobj, &S::displayStateData);
    QObject::disconnect(fobj, &F::boxChanged, sobj, &S::displayBoxData);
    QObject::disconnect(fobj, &F::transponderChanged, sobj,
                        &S::displayTransponderData);
    //    QObject::disconnect(fobj, &F::firmwareChanged, sobj,
    //    &S::displayFirmware);
  }
};

#endif // PRODUCTIONCONTEXTCONNECTOR_H
