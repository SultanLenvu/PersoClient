#ifndef PRODUCTION_GUI_H
#define PRODUCTION_GUI_H

#include <QTableView>

#include "gui.h"

/*!
 * Production GUI mode
 */
class ProductionGUI : public GUI {
 public:
  //! \cond
  QGroupBox* ControlPanelGroup;
  QVBoxLayout* ControlPanelLayout;

  QPushButton* LoadTransponderFirmwareButton;
  QSpacerItem* ButtonVerticalSpacer;
  QPushButton* ReloadTransponderFirmwareButton;
  QPushButton* PrintLastTransponderStickerButton;

  QGroupBox* TransponderInfoGroup;
  QVBoxLayout* TransponderInfoLayout;
  QTableView* TransponderInfoView;
  //! \endcond

 public:
  /*!
   * Default constructor
   * \param parent parent widget
   */
  ProductionGUI(QWidget* parent);

  /*!
   * Create widgets and layouts
   */
  virtual void create(void) override;
  /*!
   * Update sizes to match TransponderInfoView column sizes
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(ProductionGUI);
  //! \endcond
};

#endif  // PRODUCTION_GUI_H
