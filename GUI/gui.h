#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QSettings>
#include <QtWidgets>

class GUI : public QWidget {
  Q_OBJECT
 public:
  enum GuiType { Authorization, Master, Production };

 public:
  QHBoxLayout* MainLayout;

 private:
  GuiType Type;

 public:
  GUI(QWidget* parent, GuiType type);
  virtual ~GUI();

  virtual void create(void) = 0;
  virtual void update(void) = 0;

  GuiType type(void);

 signals:
};

#endif  // GUI_H
