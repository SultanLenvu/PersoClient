#include "transponder_sticker_scan_dialog.h"

TransponderStickerScanDialog::TransponderStickerScanDialog(QWidget* parent)
    : InputDialog(parent, TransponderStickerScan) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.3, DesktopGeometry.height() * 0.35,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.15);
  setWindowTitle("Сканирование стикера");
  setFixedSize(size());

  create();
}

TransponderStickerScanDialog::~TransponderStickerScanDialog() {}

void TransponderStickerScanDialog::getData(
    QHash<QString, QString>* data) const {
  if (!data) {
    return;
  }

  QStringList input = StickerData->toPlainText().split("\n");
  if (!checkInput(input)) {
    data->clear();
    return;
  }

  if (input.size() == 2) {
    data->insert("sn", input.at(0));
    data->insert("pan", input.at(1));
  } else if (input.size() == 1) {
    data->insert("pan", input.at(0));
  }
}

void TransponderStickerScanDialog::create() {
  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  MainLabel = new QLabel("Данные стикера:");
  MainLayout->addWidget(MainLabel);

  StickerData = new QPlainTextEdit();
  MainLayout->addWidget(StickerData);

  ButtonLayout = new QHBoxLayout();
  MainLayout->addLayout(ButtonLayout);

  AcceptButton = new QPushButton("Начать");
  ButtonLayout->addWidget(AcceptButton);
  connect(AcceptButton, &QPushButton::clicked, this, &QDialog::accept);

  RejectButton = new QPushButton("Отмена");
  ButtonLayout->addWidget(RejectButton);
  connect(RejectButton, &QPushButton::clicked, this, &QDialog::reject);

  MainLayoutVS =
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
  MainLayout->addItem(MainLayoutVS);
}

bool TransponderStickerScanDialog::checkInput(QStringList& input) const {
  if (input.size() == 2) {
    return checkPan(input[1]) && checkSn(input[0]);
  } else if (input.size() == 1) {
    return checkPan(input[0]);
  } else {
    return false;
  }
}

bool TransponderStickerScanDialog::checkPan(const QString& pan) const {
  if (pan.length() != PAN_CHAR_LENGTH) {
    return false;
  }

  QRegularExpression regex("^[0-9]+$");
  QRegularExpressionMatch match = regex.match(pan);
  return match.hasMatch();
}

bool TransponderStickerScanDialog::checkSn(const QString& sn) const {
  if (sn.length() != SERIAL_NUMBER_DS_CHAR_LENGTH) {
    return false;
  }

  QRegularExpression regex("^[0-9]+$");
  QRegularExpressionMatch match = regex.match(sn);
  return match.hasMatch();
}
