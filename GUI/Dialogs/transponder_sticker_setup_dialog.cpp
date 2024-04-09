#include "transponder_sticker_setup_dialog.h"
#include "definitions.h"

TransponderStickerSetupDialog::TransponderStickerSetupDialog(
    QWidget* parent)
    : AbstractInputDialog(parent) {
  setWindowTitle("Данные стикера");
  create();
}

void TransponderStickerSetupDialog::accept(void) {
  if (!checkInput()) {
    QMessageBox::critical(nullptr, "Ошибка", "Некорректный ввод данных.");
    return;
  }

  QDialog::accept();
}

void TransponderStickerSetupDialog::getData(StringDictionary& data) const {
  data.insert("issuer_name", StickerFormatChoice->currentText());
  data.insert("transponder_pan", PanInput->text());
  data.insert("transponder_sn", SnInput->text());
}

void TransponderStickerSetupDialog::create()
{
  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  StickerFormatLayout = new QHBoxLayout();
  MainLayout->addLayout(StickerFormatLayout);

  StickerFormatLabel = new QLabel("Формат стикера");
  StickerFormatLayout->addWidget(StickerFormatLabel);
  StickerFormatChoice = new QComboBox();
  StickerFormatChoice->addItem("Новое качество дорог");
  StickerFormatChoice->addItem("Магистраль северной столицы");
  StickerFormatLayout->addWidget(StickerFormatChoice);

  SnLayout = new QHBoxLayout();
  MainLayout->addLayout(SnLayout);

  SnLabel = new QLabel("Серийный номер");
  SnLayout->addWidget(SnLabel);
  SnInput = new QLineEdit();
  SnLayout->addWidget(SnInput);

  PanLayout = new QHBoxLayout();
  MainLayout->addLayout(PanLayout);

  PanLabel = new QLabel("PAN");
  PanLayout->addWidget(PanLabel);
  PanInput = new QLineEdit();
  PanLayout->addWidget(PanInput);

  ButtonLayout = new QHBoxLayout();
  MainLayout->addLayout(ButtonLayout);

  AcceptButton = new QPushButton("Распечатать");
  ButtonLayout->addWidget(AcceptButton);
  connect(AcceptButton, &QPushButton::clicked, this, &QDialog::accept);

  RejectButton = new QPushButton("Отмена");
  ButtonLayout->addWidget(RejectButton);
  connect(RejectButton, &QPushButton::clicked, this, &QDialog::reject);

  MainLayoutVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  MainLayout->addItem(MainLayoutVS);
}

bool TransponderStickerSetupDialog::checkInput() const {
  static QRegularExpression snRegex(
      QString("^[0-9]{%1}$").arg(SERIAL_NUMBER_DS_CHAR_LENGTH));
  static QRegularExpression panRegex(
      QString("^[0-9]{%1}$").arg(PAN_CHAR_LENGTH));

  QString sn = SnInput->text();
  QString pan = PanInput->text();

  if (!snRegex.match(sn).hasMatch()) {
    return false;
  }
  if (!panRegex.match(pan).hasMatch()) {
    return false;
  }

  return true;
}
