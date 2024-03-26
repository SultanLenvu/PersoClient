#include "abstract_sticker_printer.h"
#include "global_environment.h"
#include "log_system.h"

#include <QTcpSocket>

AbstractStickerPrinter::AbstractStickerPrinter(const QString& name)
    : QObject(nullptr) {
  setObjectName(name);

  QTcpSocket socket;
  socket.moveToThread();

  connect(this, &AbstractStickerPrinter::logging,
          dynamic_cast<LogSystem*>(
              GlobalEnvironment::instance()->getObject("LogSystem")),
          &LogSystem::generate);
}

AbstractStickerPrinter::~AbstractStickerPrinter() {}
