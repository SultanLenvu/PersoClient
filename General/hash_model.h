#ifndef HASH_MODEL_H
#define HASH_MODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QString>
#include <QVector>

/*!
 * \brief Transponder info model
 *
 * This class represents transponder info contained in a database
 */
class HashModel : public QAbstractTableModel {
 private:
  //! Data container
  QHash<QString, QVariant> HashTable;
  //! Table of Russian names for headers
  QHash<QString, QString> TransponderDataMatchTable;
  
  QVector<QVariant> Values;
  QVector<QVariant> Headers;

 public:
  /*!
   * Construct object, fill info match table
   * \param[in] parent QObject parent
   */
  explicit HashModel(QObject* parent);
  /*!
   * Default destructor
   */
  ~HashModel();

  /*!
   * Clear data in this model, and copy data from the argument.
   * Nothing happens if map is nullptr
   * \param[in] map Map to copy data from
   */
  void buildTransponderData(const QHash<QString, QString>* map);
  /*!
   * Clear model data
   */
  void clear(void);
  /*!
   * Check if there is no data in this model
   * \return true if this model's container is empty
   */
  bool isEmpty(void) const;
  /*!
   * Get underlying data container
   * \return Map containing data
   */
  const QHash<QString, QVariant>* map(void) const;

  /*!
   * We have at most 1 column
   * \param parent ignored
   * \return 0 if underlying container is empty, 1 otherwise
   */
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  /*!
   * Number of rows, which is equal to number of elements in
   * the underlying container
   * \param parent ignored
   * \return Number of elements in underlying container
   */
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  /*!
   * Get model data
   * \param index TODO (See Qt docs on QAbstractTableModel)
   * \param role TODO (See Qt docs on QAbstractTableModel)
   * \return QVariant() if index.column() > 1 or if index.row()
   * is more than amount of available data, or if role is not
   * Qt::DisplayRole, corresponding piece of data otherwise
   */
  QVariant data(const QModelIndex& index, int role) const override;

  /*!
   * Get header names
   * \return 
   * - QVariant() if section value is more than amount of available
   *   data or if role is not Qt::DisplayRole or if orientation is
   *   neither Qt::Horizontal or Qt::Vertical
   * - "Значение" if orientation is Qt::Vertical
   * - name of section'th column otherwise
   */
  QVariant headerData(int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole) const override;

 private:
  //! \cond
  Q_DISABLE_COPY(HashModel);
  //! \endcond
  /*!
   * Fill TransponderInfoMatchTable with Russian descriptions of
   * field names
   */
  void createMatchTables(void);
};

#endif  // HASH_MODEL_H
