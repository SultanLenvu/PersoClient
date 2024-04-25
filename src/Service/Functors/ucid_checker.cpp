#include "ucid_checker.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

bool UcidChecker::operator()(const QString& str) const {
  static QRegularExpression regex(R"(\d{32})");
  QRegularExpressionMatch match = regex.match(str);
  return match.hasMatch();
}
