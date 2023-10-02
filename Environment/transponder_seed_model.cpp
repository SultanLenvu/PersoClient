#include "transponder_seed_model.h"

TransponderInfoModel::TransponderInfoModel(QObject* parent)
    : QAbstractTableModel(parent) {
  setObjectName("TransponderInfoModel");
  Attributes = nullptr;
}

TransponderInfoModel::~TransponderInfoModel() {
  deleteAll();
}

void TransponderInfoModel::build(const QMap<QString, QString>* attributes) {
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

void TransponderInfoModel::clear() {
  beginResetModel();

  deleteAll();

  endResetModel();
}

bool TransponderInfoModel::isEmpty() {
  if (!Attributes) {
    return true;
  }

  return false;
}

const QMap<QString, QString>* TransponderInfoModel::attributes() const {
  return Attributes;
}

int TransponderInfoModel::columnCount(const QModelIndex& parent) const {
  if (!Attributes) {
    return 0;
  }

  return 1;
}

int TransponderInfoModel::rowCount(const QModelIndex& parent) const {
  if (!Attributes) {
    return 0;
  }

  return Attributes->size();
}

QVariant TransponderInfoModel::data(const QModelIndex& index, int role) const {
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

QVariant TransponderInfoModel::headerData(int section,
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

void TransponderInfoModel::deleteAll() {
  delete Attributes;
  Attributes = nullptr;
}
