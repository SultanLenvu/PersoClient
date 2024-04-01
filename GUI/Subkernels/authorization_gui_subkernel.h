#ifndef AUTHORIZATIONGUISUBKERNEL_H
#define AUTHORIZATIONGUISUBKERNEL_H

#include "abstract_gui_subkernel.h"

class AuthorizationGuiSubkernel : public AbstractGuiSubkernel {
  Q_OBJECT
 public:
  explicit AuthorizationGuiSubkernel(const QString& name);
  ~AuthorizationGuiSubkernel() = default;

 public slots:
  void logOn(void);

 public slots:
  void logOnCompleted_slot(void);

 private:
  Q_DISABLE_COPY_MOVE(AuthorizationGuiSubkernel)
  void connectDependencies(void);
};

#endif  // AUTHORIZATIONGUISUBKERNEL_H
