#ifndef IOBJECTCONNECTOR_H
#define IOBJECTCONNECTOR_H

#include <QObject>

template <typename F, typename S>
class IObjectConnector {
  static_assert(std::is_base_of_v<QObject, F>,
                "F must be derived from QObject");
  static_assert(std::is_base_of_v<QObject, S>,
                "S must be derived from QObject");

 public:
  explicit IObjectConnector() = default;
  virtual ~IObjectConnector() = default;
  Q_DISABLE_COPY_MOVE(IObjectConnector)

 public:
  virtual void plug(const F* fobj, const S* sobj) = 0;
  virtual void unplug(const F* fobj, const S* sobj) = 0;
};

#endif  // IOBJECTCONNECTOR_H
