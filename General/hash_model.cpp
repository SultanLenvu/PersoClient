#include "hash_model.h"

HashModel::HashModel(QObject* parent) : QAbstractTableModel(parent) {
  setObjectName("HashModel");
  createMatchTables();
}

HashModel::~HashModel() {}

void HashModel::buildTransponderInfo(const QHash<QString, QString>* map) {
  // Проверка на существование
  if (!map) {
    return;
  }

  beginResetModel();

  // Очищаем старые данные
  Hash.clear();

  // Устанавливаем новые данные
  for (QHash<QString, QString>::const_iterator it1 = map->constBegin();
       it1 != map->constEnd(); it1++) {
    Hash.insert(TransponderInfoMatchTable.value(it1.key()), it1.value());
  }

  endResetModel();
}

void HashModel::clear() {
  beginResetModel();

  Hash.clear();

  endResetModel();
}

bool HashModel::isEmpty() const {
  return Hash.isEmpty();
}

const QHash<QString, QVariant>* HashModel::map() const {
  return &Hash;
}

int HashModel::columnCount(const QModelIndex& parent) const {
  if (Hash.isEmpty()) {
    return 0;
  }

  return 1;
}

int HashModel::rowCount(const QModelIndex& parent) const {
  return Hash.size();
}

QVariant HashModel::data(const QModelIndex& index, int role) const {
  if (index.column() > 1) {
    return QVariant();
  }

  if (index.row() > (Hash.size())) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
    return (Hash.constBegin() + index.row()).value();
  } else
    return QVariant();
}

QVariant HashModel::headerData(int section,
                               Qt::Orientation orientation,
                               int role) const {
  if (section > (Hash.size())) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    return "Значение";
  }

  if (orientation == Qt::Vertical) {
    return (Hash.constBegin() + section).key();
  } else {
    return QVariant();
  }
}

void HashModel::createMatchTables() {
  TransponderInfoMatchTable.insert("sn", "Серийный номер");
  TransponderInfoMatchTable.insert("pan", "PAN");
  TransponderInfoMatchTable.insert("issuer_name", "Компания-заказчик");
  TransponderInfoMatchTable.insert("box_id", "Идентификатор бокса");
  TransponderInfoMatchTable.insert("pallet_id", "Идентификатор паллеты");
  TransponderInfoMatchTable.insert("order_id", "Идентификатор заказа");
  TransponderInfoMatchTable.insert("transponder_model", "Модель транспондера");
}
