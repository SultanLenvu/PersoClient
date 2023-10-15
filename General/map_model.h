#ifndef MAP_MODEL_H
#define MAP_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QString>

class MapModel : public QAbstractTableModel {
 private:
  QMap<QVariant, QVariant> Map;
  QMap<QString, QString> TransponderInfoMatchTable;

 public:
  explicit MapModel(QObject* parent);
  ~MapModel();

  void buildTransponderInfo(const QMap<QString, QString>* map);
  void clear(void);
  bool isEmpty(void) const;
  const QMap<QVariant, QVariant>* map(void) const;

  // Методы модели
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

 private:
  Q_DISABLE_COPY(MapModel);
  void createMatchTables(void);
};

#endif  // MAP_MODEL_H
