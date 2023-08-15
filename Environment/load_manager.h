#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include "../Programmers/interface_programmer.h"
#include "../Programmers/jlink_manual_programmer.h"
#include <QObject>

class LoadManager : public QObject {
  Q_OBJECT

public:
  enum LoaderStatus { Free, Success, Failed };

private:
  InterfaceProgrammer *Programmer;

public:
  explicit LoadManager(QObject *parent);

  InterfaceProgrammer *programmer(void) const;

  void processingFirmwareLoading(void);
  void processingFirmwareErasing(void);

signals:
  void logging(const QString &log);
};

#endif // LOAD_MANAGER_H
