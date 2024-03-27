#ifndef TRANSPONDERUSERDATA_H
#define TRANSPONDERUSERDATA_H

#include <QByteArray>
#include <QFile>

class TransponderUserData
{
 private:
  QByteArray Data;

 public:
  explicit TransponderUserData() = default;
  ~TransponderUserData() = default;

 public:
  bool valid(void) const;
  bool writeToFile(const QString& fileName) const;

 public:
  bool setData(QByteArray& data);
  bool readFromFile(QFile& file);
  bool readFromFile(const QString& fileName);

 private:
  bool checkFile(const QFile& file) const;
};

#endif // TRANSPONDERUSERDATA_H
