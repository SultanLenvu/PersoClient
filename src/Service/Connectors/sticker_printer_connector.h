#ifndef STICKERPRINTERCONNECTOR_H
#define STICKERPRINTERCONNECTOR_H

#include "i_object_connector.h"

template <typename F, typename S>
class StickerPrinterConnector : public IObjectConnector<F, S> {
 public:
  explicit StickerPrinterConnector() = default;
  virtual ~StickerPrinterConnector() = default;
  Q_DISABLE_COPY_MOVE(StickerPrinterConnector)

  // IObjectConnector interface
 public:
  virtual void plug(const F* fobj, const S* sobj) override {
    QObject::connect(fobj, &F::printLastTransponderSticker_signal, sobj,
                     &S::printLastTransponderSticker);
    QObject::connect(fobj, &F::printTransponderSticker_signal, sobj,
                     &S::printTransponderSticker);
    QObject::connect(fobj, &F::exec_signal, sobj, &S::exec);
  }
  virtual void unplug(const F* fobj, const S* sobj) override {
    QObject::disconnect(fobj, &F::printLastTransponderSticker_signal, sobj,
                        &S::printLastTransponderSticker);
    QObject::disconnect(fobj, &F::printTransponderSticker_signal, sobj,
                        &S::printTransponderSticker);
    QObject::disconnect(fobj, &F::exec_signal, sobj, &S::exec);
  }
};
#endif // STICKERPRINTERCONNECTOR_H
