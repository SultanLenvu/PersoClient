#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <QString>
#include <QtClassHelperMacros>

class LogMessage final {
 public:
  enum LogPriorityType {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
  };

 private:
  LogPriorityType Priority;
  QString Text;

 public:
  explicit LogMessage(const QString& txt);
  ~LogMessage() = default;

 public:
  QString toString();
};

#endif // LOGMESSAGE_H
