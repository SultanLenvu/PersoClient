#ifndef TRANSPONDER_SEED_MODEL_H
#define TRANSPONDER_SEED_MODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QString>

class TransponderInfoModel : public QAbstractTableModel {
  Q_OBJECT
 private:
  const QMap<QString, QString>* Attributes;

 public:
  explicit TransponderInfoModel(QObject* parent = nullptr);
  ~TransponderInfoModel();

  void build(const QMap<QString, QString>* attributes);
  void clear(void);
  bool isEmpty(void);
  const QMap<QString, QString>* attributes(void) const;

  // Методы модели
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;

 private:
  void deleteAll(void);
};

#endif  // TRANSPONDER_SEED_MODEL_H
