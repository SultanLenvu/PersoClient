#ifndef SERVERCONNECTIONCONNECTOR_H
#define SERVERCONNECTIONCONNECTOR_H

#include "i_object_connector.h"

template <typename F, typename S>
class ServerConnectionConnector : public IObjectConnector<F, S> {
 public:
  explicit ServerConnectionConnector() = default;
  virtual ~ServerConnectionConnector() = default;
  Q_DISABLE_COPY_MOVE(ServerConnectionConnector)

  // IObjectConnector interface
 public:
  virtual void plug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::connect_signal, sobj, &S::connect);
    QObject::connect(fobj, &F::disconnect_signal, sobj, &S::disconnect);

    QObject::connect(fobj, &F::echo_signal, sobj, &S::echo);

    QObject::connect(fobj, &F::launchProductionLine_signal, sobj,
                     &S::launchProductionLine);
    QObject::connect(fobj, &F::shutdownProductionLine_signal, sobj,
                     &S::shutdownProductionLine);
    QObject::connect(fobj, &F::getProductionLineData_signal, sobj,
                     &S::getProductionLineData);

    QObject::connect(fobj, &F::requestBox_signal, sobj, &S::requestBox);
    QObject::connect(fobj, &F::getCurrentBoxData_signal, sobj,
                     &S::getCurrentBoxData);
    QObject::connect(fobj, &F::refundCurrentBox_signal, sobj,
                     &S::refundCurrentBox);
    QObject::connect(fobj, &F::completeCurrentBox_signal, sobj,
                     &S::completeCurrentBox);

    QObject::connect(fobj, &F::releaseTransponder_signal, sobj,
                     &S::releaseTransponder);
    QObject::connect(fobj, &F::confirmTransponderRelease_signal, sobj,
                     &S::confirmTransponderRelease);
    QObject::connect(fobj, &F::rereleaseTransponder_signal, sobj,
                     &S::rereleaseTransponder);
    QObject::connect(fobj, &F::confirmTransponderRerelease_signal, sobj,
                     &S::rereleaseTransponder);
    QObject::connect(fobj, &F::rollbackTransponder_signal, sobj,
                     &S::rollbackTransponder);
    QObject::connect(fobj, &F::getCurrentTransponderData_signal, sobj,
                     &S::getCurrentTransponderData);
    QObject::connect(fobj, &F::getTransponderData_signal, sobj,
                     &S::getTransponderData);

    QObject::connect(fobj, &F::printBoxSticker_signal, sobj,
                     &S::printBoxSticker);
    QObject::connect(fobj, &F::printLastBoxSticker_signal, sobj,
                     &S::printLastBoxSticker);
    QObject::connect(fobj, &F::printPalletSticker_signal, sobj,
                     &S::printPalletSticker);
    QObject::connect(fobj, &F::printLastPalletSticker_signal, sobj,
                     &S::printLastPalletSticker);
  }
  virtual void unplug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::connect_signal, sobj, &S::connect);
    QObject::disconnect(fobj, &F::disdisconnect_signal, sobj,
                        &S::disdisconnect);

    QObject::disconnect(fobj, &F::echo_signal, sobj, &S::echo);

    QObject::disconnect(fobj, &F::launchProductionLine_signal, sobj,
                        &S::launchProductionLine);
    QObject::disconnect(fobj, &F::shutdownProductionLine_signal, sobj,
                        &S::shutdownProductionLine);
    QObject::disconnect(fobj, &F::getProductionLineData_signal, sobj,
                        &S::getProductionLineData);

    QObject::disconnect(fobj, &F::requestBox_signal, sobj, &S::requestBox);
    QObject::disconnect(fobj, &F::getCurrentBoxData_signal, sobj,
                        &S::getCurrentBoxData);
    QObject::disconnect(fobj, &F::refundCurrentBox_signal, sobj,
                        &S::refundCurrentBox);
    QObject::disconnect(fobj, &F::completeCurrentBox_signal, sobj,
                        &S::completeCurrentBox);

    QObject::disconnect(fobj, &F::releaseTransponder_signal, sobj,
                        &S::releaseTransponder);
    QObject::disconnect(fobj, &F::confirmTransponderRelease_signal, sobj,
                        &S::confirmTransponderRelease);
    QObject::disconnect(fobj, &F::rereleaseTransponder_signal, sobj,
                        &S::rereleaseTransponder);
    QObject::disconnect(fobj, &F::confirmTransponderRerelease_signal, sobj,
                        &S::rereleaseTransponder);
    QObject::disconnect(fobj, &F::rollbackTransponder_signal, sobj,
                        &S::rollbackTransponder);
    QObject::disconnect(fobj, &F::getCurrentTransponderData_signal, sobj,
                        &S::getCurrentTransponderData);
    QObject::disconnect(fobj, &F::getTransponderData_signal, sobj,
                        &S::getTransponderData);

    QObject::disconnect(fobj, &F::printBoxSticker_signal, sobj,
                        &S::printBoxSticker);
    QObject::disconnect(fobj, &F::printLastBoxSticker_signal, sobj,
                        &S::printLastBoxSticker);
    QObject::disconnect(fobj, &F::printPalletSticker_signal, sobj,
                        &S::printPalletSticker);
    QObject::disconnect(fobj, &F::printLastPalletSticker_signal, sobj,
                        &S::printLastPalletSticker);
  }
};

#endif // SERVERCONNECTIONCONNECTOR_H
