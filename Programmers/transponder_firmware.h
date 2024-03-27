#ifndef TRANSPONDERFIRMWARE_H
#define TRANSPONDERFIRMWARE_H

#include <QByteArray>
#include <QFile>

class TransponderFirmware {
 private:
  QByteArray Data;

 public:
  explicit TransponderFirmware() = default;
  ~TransponderFirmware() = default;

 public:
  size_t size(void) const;
  bool valid(void) const;
  bool writeToFile(const QString& fileName) const;

 public:
  bool setData(QByteArray& data);
  bool readFromFile(const QString& fileName);

 private:
  bool checkFile(const QFile& file) const;
};

#endif // TRANSPONDERFIRMWARE_H
