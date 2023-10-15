#ifndef STRING_MAP_MODEL_H
#define STRING_MAP_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QString>

class StringMapModel : public QAbstractTableModel {
 private:
  const QMap<QString, QString>* Map;

 public:
  explicit StringMapModel(QObject* parent);
  ~StringMapModel();

  void build(const QMap<QString, QString>* map);
  void clear(void);
  bool isEmpty(void) const;
  const QMap<QString, QString>* map(void) const;

  // Методы модели
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

 private:
  Q_DISABLE_COPY(StringMapModel);
  void deleteAll(void);
};

#endif  // STRING_MAP_MODEL_H
