#ifndef ABSTRACTGUISUBKERNEL_H
#define ABSTRACTGUISUBKERNEL_H

#include <QObject>

#include "named_object.h"

class AbstractGuiSubkernel : public NamedObject {
  Q_OBJECT
 public:
  explicit AbstractGuiSubkernel(const QString& name);
  virtual ~AbstractGuiSubkernel() = default;

 private:
  Q_DISABLE_COPY_MOVE(AbstractGuiSubkernel);

 signals:
  void clearLogDisplay_signal(void);
};

#endif  // ABSTRACTGUISUBKERNEL_H
