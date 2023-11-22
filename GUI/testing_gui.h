#ifndef TestingGUI_H
#define TestingGUI_H

#include "abstract_gui.h"

/*!
 * Testing interface
 */
class TestingGUI : public AbstractGUI {
 public:
  //! \cond
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* ReloadTransponderFirmwareButton;

  QGroupBox* PrintStickerGroup;
  QVBoxLayout* PrintStickerLayout;
  QPushButton* PrintLastTransponderStickerButton;
  QPushButton* PrintCustomTransponderStickerButton;
  QPushButton* PrintBoxStickerButton;
  QPushButton* PrintPalletStickerButton;

  QSpacerItem* ControlPanelVS;

  QGroupBox* TransponderDataGroup;
  QVBoxLayout* TransponderDataLayout;
  QTableView* TransponderDataView;
  //! \endcond
 public:
  /*!
   * Construct object, create layout
   * \param parent Parent layout
   */
  explicit TestingGUI(QWidget* parent);
  /*!
   * Default no-op destructor
   */
  ~TestingGUI();

  /*!
   * Update size to match contents width
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(TestingGUI);
  //! \endcond
};

#endif  // TestingGUI_H
