#ifndef TestingGui_H
#define TestingGui_H

#include "abstract_gui.h"

/*!
 * Testing interface
 */
class TestingGui : public AbstractGui {
 public:
  //! \cond
  QVBoxLayout* ControlPanelLayout;

  QGroupBox* TransponderGroup;
  QVBoxLayout* TransponderLayout;
  QPushButton* RereleaseTransponderButton;

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
  explicit TestingGui(QWidget* parent);
  /*!
   * Default no-op destructor
   */
  ~TestingGui();

  /*!
   * Update size to match contents width
   */
  virtual void update(void) override;

 private:
  //! \cond
  Q_DISABLE_COPY(TestingGui);
  //! \endcond
};

#endif  // TestingGui_H
