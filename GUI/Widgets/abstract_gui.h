#ifndef ABSTRACT_GUI_H
#define ABSTRACT_GUI_H

#include <QSettings>
#include <QtWidgets>

class AbstractGui : public QWidget {
  Q_OBJECT
 public:
  enum GuiType {
    Authorization,
    Master,
    ProductionAssembler,
    ProductionTester,
  };

 public:
  QHBoxLayout* MainLayout;

 public:
  AbstractGui(QWidget* parent);
  virtual ~AbstractGui();

  virtual void updateModelViews(void) = 0;
  virtual GuiType type(void) = 0;

 private:
  Q_DISABLE_COPY_MOVE(AbstractGui);

 signals:
  void visibilityChanged(void);
};

#endif  // ABSTRACT_GUI_H
