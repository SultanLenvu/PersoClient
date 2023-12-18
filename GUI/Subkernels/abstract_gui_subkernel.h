#ifndef ABSTRACTGUISUBKERNEL_H
#define ABSTRACTGUISUBKERNEL_H

#include <QObject>

#include "abstract_gui.h"
#include "abstract_manager.h"

class AbstractGuiSubkernel : public QObject {
  Q_OBJECT
 public:
  explicit AbstractGuiSubkernel(const QString& name);
  virtual ~AbstractGuiSubkernel();

  virtual void setCurrentGui(std::shared_ptr<AbstractGui> gui) = 0;
  virtual void setManager(std::shared_ptr<AbstractManager> manager) = 0;

 private:
  AbstractGuiSubkernel();
  Q_DISABLE_COPY_MOVE(AbstractGuiSubkernel);

 signals:
  void loggerClear_signal(void);
};

#endif  // ABSTRACTGUISUBKERNEL_H
