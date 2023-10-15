#include "qmap_model.h"

StringMapModel::StringMapModel(QObject* parent) : QAbstractTableModel(parent) {
  setObjectName("StringMapModel");
  Map = nullptr;
}

StringMapModel::~StringMapModel() {
  deleteAll();
}

void StringMapModel::build(const QMap<QString, QString>* map) {
  // Проверка на существование
  if (!map) {
    return;
  }

  beginResetModel();

  // Очищаем старые данные
  deleteAll();

  // Устанавливаем новые данные
  Map = map;

  endResetModel();
}

void StringMapModel::clear() {
  beginResetModel();

  deleteAll();

  endResetModel();
}

bool StringMapModel::isEmpty() const {
  if (!Map) {
    return true;
  }

  return false;
}

const QMap<QString, QString>* StringMapModel::map() const {
  return Map;
}

int StringMapModel::columnCount(const QModelIndex& parent) const {
  if (!Map) {
    return 0;
  }

  return 1;
}

int StringMapModel::rowCount(const QModelIndex& parent) const {
  if (!Map) {
    return 0;
  }

  return Map->size();
}

QVariant StringMapModel::data(const QModelIndex& index, int role) const {
  if (!Map) {
    return QVariant();
  }

  if (index.column() > 1) {
    return QVariant();
  }

  if (index.row() > (Map->size())) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
    return (Map->constBegin() + index.row()).value();
  } else
    return QVariant();
}

QVariant StringMapModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const {
  if (!Map) {
    return QVariant();
  }

  if (section > (Map->size())) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    return "Значение";
  }

  if (orientation == Qt::Vertical) {
    return (Map->constBegin() + section).key();
  } else {
    return QVariant();
  }
}

void StringMapModel::deleteAll() {
  delete Map;
  Map = nullptr;
}
