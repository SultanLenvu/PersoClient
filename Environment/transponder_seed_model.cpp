#include "transponder_seed_model.h"

TransponderSeedModel::TransponderSeedModel(QObject* parent)
    : QAbstractTableModel(parent) {
  setObjectName("TransponderSeedModel");
  Attributes = nullptr;
}

TransponderSeedModel::~TransponderSeedModel() {
  deleteAll();
}

void TransponderSeedModel::build(const QMap<QString, QString>* attributes) {
  // Проверка на существование
  if (!attributes) {
    return;
  }

  beginResetModel();

  // Очищаем старые данные
  deleteAll();

  // Устанавливаем новые данные
  Attributes = attributes;

  endResetModel();
}

void TransponderSeedModel::clear() {
  beginResetModel();

  deleteAll();

  endResetModel();
}

bool TransponderSeedModel::isEmpty() {
  if (!Attributes) {
    return true;
  }

  return false;
}

const QMap<QString, QString>* TransponderSeedModel::attributes() const {
  return Attributes;
}

int TransponderSeedModel::columnCount(const QModelIndex& parent) const {
  if (!Attributes) {
    return 0;
  }

  return 1;
}

int TransponderSeedModel::rowCount(const QModelIndex& parent) const {
  if (!Attributes) {
    return 0;
  }

  return Attributes->size();
}

QVariant TransponderSeedModel::data(const QModelIndex& index, int role) const {
  if (!Attributes) {
    return QVariant();
  }

  if (index.column() > 1) {
    return QVariant();
  }

  if (index.row() > (Attributes->size())) {
    return QVariant();
  }

  if (role == Qt::DisplayRole) {
      return (Attributes->constBegin() + index.row()).value();
  } else
    return QVariant();
}

QVariant TransponderSeedModel::headerData(int section,
                                          Qt::Orientation orientation,
                                          int role) const {
  if (!Attributes) {
    return QVariant();
  }

  if (section > (Attributes->size())) {
    return QVariant();
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    return "Значение";
  }

  if (orientation == Qt::Vertical) {
      return (Attributes->constBegin() + section).key();
  } else {
    return QVariant();
  }
}

void TransponderSeedModel::deleteAll() {
  delete Attributes;
  Attributes = nullptr;
}
