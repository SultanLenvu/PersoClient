#include "custom_transponder_sticker_dialog.h"

CustomTransponderStickerScanDialog::CustomTransponderStickerScanDialog(
    QWidget* parent)
    : QDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5,
              DesktopGeometry.width() * 0.2, DesktopGeometry.height() * 0.1);
  setWindowTitle("Данные стикера");

  MainLayout = new QGridLayout();
  setLayout(MainLayout);

  SnLabel = new QLabel("Серийный номер");
  MainLayout->addWidget(SnLabel, 3, 0, 1, 1);
  SnInput = new QLineEdit();
  MainLayout->addWidget(SnInput, 3, 1, 1, 1);

  PanLabel = new QLabel("PAN");
  MainLayout->addWidget(PanLabel, 4, 0, 1, 1);
  PanInput = new QLineEdit();
  MainLayout->addWidget(PanInput, 4, 1, 1, 1);

  OkButton = new QPushButton("Распечатать");
  MainLayout->addWidget(OkButton, 5, 0, 1, 2);
  connect(OkButton, &QPushButton::clicked, this, &QDialog::accept);

  CancelButton = new QPushButton("Отмена");
  MainLayout->addWidget(CancelButton, 6, 0, 1, 2);
  connect(CancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CustomTransponderStickerScanDialog::getData(
    QHash<QString, QString>* data) {
  if (!data) {
    return;
  }

  if (!checkInput()) {
    return;
  }

  data->insert("pan", PanInput->text());
  data->insert("sn", SnInput->text());
}

bool CustomTransponderStickerScanDialog::checkInput() {
  QString sn = SnInput->text();
  QString pan = PanInput->text();

  if ((sn.size() != SERIAL_NUMBER_DS_CHAR_LENGTH) || (sn.toInt() == 0)) {
    return false;
  }

  if (pan.size() != PAN_CHAR_LENGTH) {
    return false;
  }

  QRegExp regex("\\D");
  if (regex.indexIn(pan) != -1) {
    return false;
  }

  return true;
}
