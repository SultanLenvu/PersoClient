#include "transponder_sticker_scan_dialog.h"

TransponderStickerScanDialog::TransponderStickerScanDialog(QWidget* parent)
    : QDialog(parent) {
  // Считываем размеры дисплея
  DesktopGeometry = QApplication::desktop()->screenGeometry();

  // Создаем диалоговое окно
  setGeometry(DesktopGeometry.width() * 0.5, DesktopGeometry.height() * 0.5,
              DesktopGeometry.width() * 0.1, DesktopGeometry.height() * 0.1);
  setWindowTitle("Перевыпуск по PAN");

  MainLayout = new QVBoxLayout();
  setLayout(MainLayout);

  MainLabel = new QLabel("Отсканируйте стикер транспондера");
  MainLayout->addWidget(MainLabel);

  StickerDataInput = new QPlainTextEdit();
  MainLayout->addWidget(StickerDataInput);

  OkButton = new QPushButton("Начать");
  MainLayout->addWidget(OkButton);
  connect(OkButton, &QPushButton::clicked, this, &QDialog::accept);

  CancelButton = new QPushButton("Отмена");
  MainLayout->addWidget(CancelButton);
  connect(CancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void TransponderStickerScanDialog::getData(QStringList* data) {
  if (!data) {
    return;
  }

  if (StickerDataInput->toPlainText().isEmpty()) {
    return;
  }
  *data = StickerDataInput->toPlainText().split("\n");
}
