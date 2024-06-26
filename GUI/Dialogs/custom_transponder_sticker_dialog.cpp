#include "custom_transponder_sticker_dialog.h"
#include "General/definitions.h"

CustomTransponderStickerScanDialog::CustomTransponderStickerScanDialog(
    QWidget* parent)
    : AbstractInputDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::primaryScreen()->size();

  create();
  createMatchTable();

  Regex.setPattern("^[0-9]+$");

  setWindowTitle("Данные стикера");
}

CustomTransponderStickerScanDialog::~CustomTransponderStickerScanDialog() {}

AbstractInputDialog::InputDialogType CustomTransponderStickerScanDialog::type()
    const {
  return CustomTransponderSticker;
}

void CustomTransponderStickerScanDialog::getData(StringDictionary& data) const {
  if (!checkInput()) {
    return;
  }

  data.insert("issuer_name",
              MatchTable.value(StickerFormatChoice->currentText()));
  data.insert("pan", PanInput->text());
  data.insert("sn", SnInput->text());
}

void CustomTransponderStickerScanDialog::create()
{
  MainLayout = new QGridLayout();
  setLayout(MainLayout);

  StickerFormatLayout = new QHBoxLayout();
  MainLayout->addLayout(StickerFormatLayout, 0, 0, 1, 1);

  StickerFormatLabel = new QLabel("Формат стикера");
  StickerFormatLayout->addWidget(StickerFormatLabel);
  StickerFormatChoice = new QComboBox();
  StickerFormatChoice->addItem("Квадрат");
  StickerFormatChoice->addItem("Прямоугольник");
  StickerFormatLayout->addWidget(StickerFormatChoice);

  SnLayout = new QHBoxLayout();
  MainLayout->addLayout(SnLayout, 1, 0, 1, 1);

  SnLabel = new QLabel("Серийный номер");
  SnLayout->addWidget(SnLabel);
  SnInput = new QLineEdit();
  SnLayout->addWidget(SnInput);

  PanLayout = new QHBoxLayout();
  MainLayout->addLayout(PanLayout, 2, 0, 1, 1);

  PanLabel = new QLabel("PAN");
  PanLayout->addWidget(PanLabel);
  PanInput = new QLineEdit();
  PanLayout->addWidget(PanInput);

  ButtonLayout = new QHBoxLayout();
  MainLayout->addLayout(ButtonLayout, 3, 0, 1, 1);

  AcceptButton = new QPushButton("Распечатать");
  ButtonLayout->addWidget(AcceptButton);
  connect(AcceptButton, &QPushButton::clicked, this, &QDialog::accept);

  RejectButton = new QPushButton("Отмена");
  ButtonLayout->addWidget(RejectButton);
  connect(RejectButton, &QPushButton::clicked, this, &QDialog::reject);

  MainLayoutVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  MainLayout->addItem(MainLayoutVS, 4, 0, 1, 1);
}

void CustomTransponderStickerScanDialog::createMatchTable()
{
  MatchTable.insert("Квадрат", "Новое качество дорог");
  MatchTable.insert("Прямоугольник", "Магистраль северной столицы");
}

bool CustomTransponderStickerScanDialog::checkInput() const
{
  QString sn = SnInput->text();
  QString pan = PanInput->text();

  if (sn.size() != SERIAL_NUMBER_DS_CHAR_LENGTH) {
    return false;
  }

  if (pan.size() != PAN_CHAR_LENGTH) {
    return false;
  }

  if (!Regex.match(sn).hasMatch()) {
    return false;
  }
  if (!Regex.match(sn).hasMatch()) {
    return false;
  }

  return true;
}
