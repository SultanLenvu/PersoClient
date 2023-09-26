#include "te310_printer.h"

TE310Printer::TE310Printer(QObject* parent) : IStickerPrinter(parent, TE310) {
  loadSetting();

  Driver = new QLibrary(DriverPath, this);
}

void TE310Printer::print(const QMap<QString, QString>* parameters) {
  emit logging("Начало печати.");

  typedef int (*TSCabout)();
  typedef int (*TSCopenport)(const char*);
  typedef int (*TSCsendcommand)(const char*);
  typedef int (*TSCcloseport)();

  if (Driver->load()) {
    emit logging("Печать.");
    TSCabout about =
        (TSCabout)Driver->resolve("about");  // add about() function
    TSCopenport openport =
        (TSCopenport)Driver->resolve("openport");  // add about() function
    TSCcloseport closeport =
        (TSCcloseport)Driver->resolve("closeport");  // add closeport() function
    TSCsendcommand sendcommand =
        (TSCsendcommand)Driver->resolve("sendcommand");  // add about() function

    // about();
    openport("TSC TE310");
    sendcommand("SIZE 100 mm. 60 mm");
    sendcommand("DIRECTION 1");
    sendcommand("CLS");
    sendcommand("TEXT 100,50,\"3\",0,1,1,\"12345\"");
    sendcommand("BARCODE 100,250, \"128\",70,1,0,2,2, \"123456\"");
    sendcommand("PRINT 1");
    closeport();
  }
  emit logging("Печать завершена.");
}

void TE310Printer::applySetting() {
  emit logging("Применение новых настроек.");

  loadSetting();
  delete Driver;
  Driver = new QLibrary(DriverPath, this);
}

void TE310Printer::loadSetting() {
  QSettings settings;

  DriverPath = settings.value("StickerPrinter/DLL/Path").toString();
}
