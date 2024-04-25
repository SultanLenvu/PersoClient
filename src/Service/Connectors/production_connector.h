#ifndef PRODUCTIONCONNECTOR_H
#define PRODUCTIONCONNECTOR_H

#include "i_object_connector.h"

template <typename F, typename S>
class ProductionConnector final : public IObjectConnector<F, S> {
 public:
  explicit ProductionConnector() = default;
  ~ProductionConnector() = default;
  Q_DISABLE_COPY_MOVE(ProductionConnector)

 public:
  virtual void plug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::logOn_signal, sobj, &S::logOn);
    QObject::connect(fobj, &F::logOut_signal, sobj, &S::logOut);

    QObject::connect(fobj, &F::requestBox_signal, sobj, &S::requestBox);
    QObject::connect(fobj, &F::refundCurrentBox_signal, sobj,
                     &S::refundCurrentBox);
    QObject::connect(fobj, &F::completeCurrentBox_signal, sobj,
                     &S::completeCurrentBox);

    QObject::connect(fobj, &F::releaseTransponder_signal, sobj,
                     &S::releaseTransponder);
    QObject::connect(fobj, &F::rereleaseTransponder_signal, sobj,
                     &S::rereleaseTransponder);
    QObject::connect(fobj, &F::rollbackTransponder_signal, sobj,
                     &S::rollbackTransponder);
  }

  virtual void unplug(const F* fobj, const S* sobj) override {
    QObject::disconnect(fobj, &F::logOn_signal, sobj, &S::logOn);
    QObject::disconnect(fobj, &F::logOut_signal, sobj, &S::logOut);

    QObject::disconnect(fobj, &F::requestBox_signal, sobj, &S::requestBox);
    QObject::disconnect(fobj, &F::refundCurrentBox_signal, sobj,
                        &S::refundCurrentBox);
    QObject::disconnect(fobj, &F::completeCurrentBox_signal, sobj,
                        &S::completeCurrentBox);

    QObject::disconnect(fobj, &F::releaseTransponder_signal, sobj,
                        &S::releaseTransponder);
    QObject::disconnect(fobj, &F::rereleaseTransponder_signal, sobj,
                        &S::rereleaseTransponder);
    QObject::disconnect(fobj, &F::rollbackTransponder_signal, sobj,
                        &S::rollbackTransponder);
  }
};

#endif // PRODUCTIONCONNECTOR_H
