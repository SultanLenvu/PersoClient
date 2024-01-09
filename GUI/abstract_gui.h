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
    Production,
    Testing,
  };

 public:
  QHBoxLayout* MainLayout;

 private:
  GuiType Type;

 public:
  AbstractGui(QWidget* parent, GuiType type);
  virtual ~AbstractGui();

  virtual void update(void) = 0;

  GuiType type(void);

 private:
  Q_DISABLE_COPY_MOVE(AbstractGui);

 signals:
  void visibilityChanged(void);
};

#endif  // ABSTRACT_GUI_H
