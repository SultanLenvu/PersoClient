#ifndef ABSTRACTPROGRAMMER_H
#define ABSTRACTPROGRAMMER_H

#include <QDir>
#include <QFile>
#include <QObject>
#include <QString>

#include "../Environment/definitions.h"

class InterfaceProgrammer : public QObject
{
  Q_OBJECT

protected:
  QFileInfo* FirmwareFileInfo;
  QFile* FirmwareFile;

public:
  explicit InterfaceProgrammer(QObject* parent);

  virtual void setFirmwareFile(const QString& path);

  virtual void loadFirmware(void) = 0;
  virtual void eraseFirmware(void) = 0;

private:
  void processingFirmwarePath(const QString& path);

signals:
  void logging(const QString& log);
};

#endif // ABSTRACTPROGRAMMER_H
