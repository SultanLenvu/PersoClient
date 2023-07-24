#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QtWidgets>

class GUI : public QObject {
  Q_OBJECT
public:
  enum GuiType { Master, Production };

public:
  QWidget *MainWidget;
  QHBoxLayout *MainLayout;

private:
  GuiType Type;

public:
  GUI(QObject *parent, GuiType type);
  virtual ~GUI();

  virtual QWidget *create(void) = 0;
  virtual void update(void) = 0;

  GuiType type(void);

signals:
};

#endif // GUI_H
