#include "custom_transponder_sticker_dialog.h"

CustomTransponderStickerScanDialog::CustomTransponderStickerScanDialog(
    QWidget* parent)
    : QDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5,
              DesktopGeometry.width() * 0.3, DesktopGeometry.height() * 0.1);
  setWindowTitle("Данные стикера");

  MainLayout = new QGridLayout();
  setLayout(MainLayout);

  IssuerLabel = new QLabel("Заказчик");
  MainLayout->addWidget(IssuerLabel, 0, 0, 1, 1);
  IssuerInput = new QLineEdit();
  MainLayout->addWidget(IssuerInput, 0, 1, 1, 1);

  ManufacturerIdLabel = new QLabel("Идентификатор производителя");
  MainLayout->addWidget(ManufacturerIdLabel, 1, 0, 1, 1);
  ManufacturerIdInput = new QLineEdit();
  MainLayout->addWidget(ManufacturerIdInput, 1, 1, 1, 1);

  BatteryInsertationDateLabel = new QLabel("Дата производства");
  MainLayout->addWidget(BatteryInsertationDateLabel, 2, 0, 1, 1);
  BatteryInsertationDateInput = new QLineEdit();
  MainLayout->addWidget(BatteryInsertationDateInput, 2, 1, 1, 1);

  SnLabel = new QLabel("Серийный номер");
  MainLayout->addWidget(SnLabel, 3, 0, 1, 1);
  SnInput = new QLineEdit();
  MainLayout->addWidget(SnInput, 3, 1, 1, 1);

  PanLabel = new QLabel("PAN");
  MainLayout->addWidget(PanLabel, 4, 0, 1, 1);
  PanInput = new QLineEdit();
  MainLayout->addWidget(PanInput, 4, 1, 1, 1);

  OkButton = new QPushButton("Начать");
  MainLayout->addWidget(OkButton, 5, 0, 1, 2);
  connect(OkButton, &QPushButton::clicked, this, &QDialog::accept);

  CancelButton = new QPushButton("Отмена");
  MainLayout->addWidget(CancelButton, 6, 0, 1, 2);
  connect(CancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CustomTransponderStickerScanDialog::getData(QMap<QString, QString>* data) {
  if (!data) {
    return;
  }

  if (!checkInput()) {
    return;
  }

  data->insert("issuer_name", IssuerInput->text());
  data->insert("pan", PanInput->text());
  data->insert("sn", SnInput->text());
  data->insert("battery_insertation_date", BatteryInsertationDateInput->text());
  data->insert("manufacturer_id", ManufacturerIdInput->text());
}

bool CustomTransponderStickerScanDialog::checkInput() {
  QString manufacturerId = ManufacturerIdInput->text();
  QString batteryInsertationDate = BatteryInsertationDateInput->text();
  QString sn = SnInput->text();
  QString pan = PanInput->text();

  if ((manufacturerId.size() != MANUFACTURER_ID_DS_CHAR_LENGTH) ||
      (manufacturerId.toInt() == 0)) {
    return false;
  }

  if ((batteryInsertationDate.size() !=
       BATTERY_INSERTATION_DATE_DS_CHAR_LENGTH) ||
      (batteryInsertationDate.toInt() == 0)) {
    return false;
  }

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
