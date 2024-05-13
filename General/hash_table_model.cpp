#include "hash_table_model.h"

HashTableModel::HashTableModel(const QString& name)
    : QAbstractTableModel(nullptr) {
  setObjectName(name);
}

HashTableModel::~HashTableModel() {}

void HashTableModel::setData(const StringDictionary& table) {
  beginResetModel();

  // Очищаем старые данные
  Values.clear();
  Headers.clear();

  // Устанавливаем новые данные
  for (auto itb = table.constBegin(), ite = table.constEnd(); itb != ite;
       ++itb) {
    Values.append(itb.value());
    Headers.append(MatchTable->value(itb.key()));
  }

  endResetModel();
}

void HashTableModel::setMatchTable(std::shared_ptr<StringDictionary> match) {
  MatchTable = match;
}

void HashTableModel::clear() {
  beginResetModel();

  Values.clear();
  Headers.clear();

  endResetModel();
}

bool HashTableModel::isEmpty() const {
  return Values.isEmpty();
}

int HashTableModel::columnCount(const QModelIndex& parent) const {
  if (Values.isEmpty()) {
    return 0;
  }

  return 1;
}

int HashTableModel::rowCount(const QModelIndex& parent) const {
  return Values.size();
}

QVariant HashTableModel::data(const QModelIndex& index, int role) const {
  if (index.column() > 1) {
    return QVariant();
  }

  if (index.row() > (Values.size())) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
    return Values.at(index.row());
  } else
    return QVariant();
}

QVariant HashTableModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const {
  if (section > (Values.size())) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    return "Значение";
  }

  if (orientation == Qt::Vertical) {
    return Headers.at(section);
  } else {
    return QVariant();
  }
}
