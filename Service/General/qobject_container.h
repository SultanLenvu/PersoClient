#ifndef QOBJECTCONTAINER_H
#define QOBJECTCONTAINER_H

#include <QHash>
#include <QObject>

class QObjectContainer : public QObject {
  Q_OBJECT

 private:
  QHash<QString, QObject*> Objects;

 public:
  explicit QObjectContainer() = default;
  ~QObjectContainer() = default;
  Q_DISABLE_COPY_MOVE(QObjectContainer)

 public:
  void remove(const QString& name);
  void add(QObject* obj);

  template <typename T>
  T* get(const QString& name) {
    static_assert(std::is_base_of_v<QObject, T>,
                  "T must be derived from QObject");

    if (!Objects.contains(name)) {
      return nullptr;
    }

    if (!Objects.value(name)) {
      Objects.remove(name);
      return nullptr;
    }

    return static_cast<T*>(Objects.value(name));
  }

 private slots:
  void removeSlot(void);
};

#endif  // QOBJECTCONTAINER_H
