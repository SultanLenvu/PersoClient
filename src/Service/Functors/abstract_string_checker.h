#ifndef ABSTRACTSTRINGCHECKER_H
#define ABSTRACTSTRINGCHECKER_H

#include <QString>

class AbstractStringChecker {
 public:
  explicit AbstractStringChecker() = default;
  virtual ~AbstractStringChecker() = default;

 public:
  virtual bool operator()(const QString&) const = 0;

 private:
  Q_DISABLE_COPY_MOVE(AbstractStringChecker)
};

#endif  // ABSTRACTSTRINGCHECKER_H
