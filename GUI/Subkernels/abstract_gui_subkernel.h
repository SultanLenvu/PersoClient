#ifndef ABSTRACTGUISUBKERNEL_H
#define ABSTRACTGUISUBKERNEL_H

#include <QObject>

#include "types.h"

class AbstractGuiSubkernel : public QObject {
  Q_OBJECT
 public:
  explicit AbstractGuiSubkernel();
  virtual ~AbstractGuiSubkernel() = default;
  Q_DISABLE_COPY_MOVE(AbstractGuiSubkernel);

 signals:
  void clearLogDisplay_signal(void);
  void returnStatusReady(ReturnStatus ret);
  void operationStarted(const QString& name);
  void operationFinished(const QString& name);
};

#endif  // ABSTRACTGUISUBKERNEL_H
