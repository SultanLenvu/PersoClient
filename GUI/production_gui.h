#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QTableView>

#include "abstract_gui.h"

/*!
 * Production GUI mode
 */
class ProductionGUI : public AbstractGui {
 public:
  //! \cond
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* LoadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* RollbackProductionLinePushButton;
  QPushButton* ReloadTransponderFirmwareButton;

  QGroupBox* PrintStickerGroup;
  QVBoxLayout* PrintStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;
  //! \endcond

 public:
  /*!
   * Default constructor. Initialize widgets and layout
   * \param parent Parent widget
   */
  explicit ProductionGUI(QWidget* parent);
  /*!
   * Default destructor
   */
  ~ProductionGUI();

  /*!
   * Update sizes to match TransponderDataView column sizes
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(ProductionGUI);
  //! \endcond
};

#endif  // PRODUCTION_GUI_H
