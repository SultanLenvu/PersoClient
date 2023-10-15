#include "map_model.h"

MapModel::MapModel(QObject* parent) : QAbstractTableModel(parent) {
  setObjectName("MapModel");
  createMatchTables();
}

MapModel::~MapModel() {}

void MapModel::buildTransponderInfo(const QMap<QString, QString>* map) {
  // Проверка на существование
  if (!map) {
    return;
  }

  beginResetModel();

  // Очищаем старые данные
  Map.clear();

  // Устанавливаем новые данные
  for (QMap<QString, QString>::const_iterator it1 = map->constBegin();
       it1 != map->constEnd(); it1++) {
    Map.insert(TransponderInfoMatchTable.value(it1.key()), it1.value());
  }

  endResetModel();
}

void MapModel::clear() {
  beginResetModel();

  Map.clear();

  endResetModel();
}

bool MapModel::isEmpty() const {
  return Map.isEmpty();
}

const QMap<QVariant, QVariant>* MapModel::map() const {
  return &Map;
}

int MapModel::columnCount(const QModelIndex& parent) const {
  if (Map.isEmpty()) {
    return 0;
  }

  return 1;
}

int MapModel::rowCount(const QModelIndex& parent) const {
  return Map.size();
}

QVariant MapModel::data(const QModelIndex& index, int role) const {
  if (index.column() > 1) {
    return QVariant();
  }

  if (index.row() > (Map.size())) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
    return (Map.constBegin() + index.row()).value();
  } else
    return QVariant();
}

QVariant MapModel::headerData(int section,
                              Qt::Orientation orientation,
                              int role) const {
  if (section > (Map.size())) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    return "Значение";
  }

  if (orientation == Qt::Vertical) {
    return (Map.constBegin() + section).key();
  } else {
    return QVariant();
  }
}

void MapModel::createMatchTables() {
  TransponderInfoMatchTable.insert("sn", "Серийный номер");
  TransponderInfoMatchTable.insert("pan", "PAN");
  TransponderInfoMatchTable.insert("issuer_name", "Компания-заказчик");
  TransponderInfoMatchTable.insert("box_id", "Идентификатор бокса");
  TransponderInfoMatchTable.insert("pallet_id", "Идентификатор паллеты");
  TransponderInfoMatchTable.insert("order_id", "Идентификатор заказа");
  TransponderInfoMatchTable.insert("transponder_model", "Модель транспондера");
}
