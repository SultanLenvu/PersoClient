#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QHash>
#include <QtWidgets>

#include "General/definitions.h"

/*!
 * Base class for dialogs
 */
class InputDialog : public QDialog
{
  Q_OBJECT
 public:
  /*!
   * Dialog subclass types
   */
  enum DialogType {
    MasterPassword,
    Authorization,
    TransponderStickerScan,
    CustomTransponderSticker,
    PanInput,
  };
  //! \cond
  Q_ENUM(DialogType);
  //! \endcond

 protected:
  /*!
   * Subclass type
   */
  DialogType Type;

 public:
   /*!
    * Construct dialog
    * \param[in] parent Parent widget
    * \param[in] type Subclass type. Should be set by subclass constructor
    */
   explicit InputDialog(QWidget* parent, DialogType type);
   /*!
    * Default destructor
    */
   virtual ~InputDialog();

   /*!
    * Get user input from this dialog
    * \param[out] data Map to store data in
    */
   virtual void getData(QHash<QString, QString>* data) const = 0;

   /*!
    * Get subclass type
    * \return subclass type
    */
   DialogType type(void) const;

 private:
   //! \cond
   Q_DISABLE_COPY(InputDialog);
   //! \endcond
};

#endif  // INPUTDIALOG_H
