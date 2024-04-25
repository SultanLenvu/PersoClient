#include "production_manager_gui_subkernel.h"
#include "async_production_manager.h"
#include "authorization_dialog.h"
#include "global_environment.h"
#include "production_unit_context.h"
#include "transponder_sticker_scan_dialog.h"

ProductionManagerGuiSubkernel::ProductionManagerGuiSubkernel() {
  createModels();
}

HashTableModel& ProductionManagerGuiSubkernel::productionLineModel() {
  return ProductionLineModel;
}

HashTableModel& ProductionManagerGuiSubkernel::boxModel() {
  return BoxModel;
}

HashTableModel& ProductionManagerGuiSubkernel::transponderModel() {
  return TransponderModel;
}

void ProductionManagerGuiSubkernel::logOn(const StringDictionary& param) {}

void ProductionManagerGuiSubkernel::logOn() {
  StringDictionary param;

  AuthorizationDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit logOn_signal(param);
}

void ProductionManagerGuiSubkernel::logOut() {
  emit clearLogDisplay_signal();
  emit logOut_signal();
}

void ProductionManagerGuiSubkernel::requestBox() {
  emit clearLogDisplay_signal();
  emit requestBox_signal();
}

void ProductionManagerGuiSubkernel::refundCurrentBox() {
  emit clearLogDisplay_signal();
  emit refundCurrentBox_signal();
}

void ProductionManagerGuiSubkernel::completeCurrentBox() {
  emit clearLogDisplay_signal();
  emit completeCurrentBox_signal();
}

void ProductionManagerGuiSubkernel::releaseTransponder() {
  emit clearLogDisplay_signal();
  emit releaseTransponder_signal();
}

void ProductionManagerGuiSubkernel::rereleaseTransponder() {
  StringDictionary param;

  TransponderStickerScanDialog dialog;
  if (dialog.exec() == QDialog::Rejected) {
    return;
  }
  dialog.getData(param);

  emit clearLogDisplay_signal();
  emit rereleaseTransponder_signal(param);
}

void ProductionManagerGuiSubkernel::rollbackTransponder() {
  emit clearLogDisplay_signal();
  emit rollbackTransponder_signal();
}

void ProductionManagerGuiSubkernel::displayStateData(
    const StringDictionary& data) {
  ProductionLineModel.setData(data);
}

void ProductionManagerGuiSubkernel::displayBoxData(
    const StringDictionary& data) {
  BoxModel.setData(data);
}

void ProductionManagerGuiSubkernel::displayTransponderData(
    const StringDictionary& data) {
  TransponderModel.setData(data);
}


void ProductionManagerGuiSubkernel::createModels() {
  StringDictionary plMatchTable;
  plMatchTable.insert("production_line_id", "Идентификатор");
  plMatchTable.insert("production_line_login", "Логин");
  plMatchTable.insert("production_line_in_process", "В процессе сборки");
  plMatchTable.insert("production_line_ns", "Сотрудник");
  plMatchTable.insert("today_assembled_boxes", "Собрано боксов за сегодня");
  plMatchTable.insert("transponder_id", "Идентификатор транспондера");
  plMatchTable.insert("box_id", "Идентификатор бокса");

  ProductionLineModel.setMatchTable(plMatchTable);

  StringDictionary tMatchTable;
  tMatchTable.insert("transponder_sn", "Серийный номер");
  tMatchTable.insert("transponder_ucid", "UCID");
  tMatchTable.insert("transponder_pan", "PAN");
  tMatchTable.insert("box_id", "Идентификатор бокса");
  tMatchTable.insert("transponder_release_counter", "Количество выпусков");
  tMatchTable.insert("issuer_name", "Заказчик");

  TransponderModel.setMatchTable(tMatchTable);

  StringDictionary bMatchTable;
  bMatchTable.insert("box_id", "ID бокса");
  bMatchTable.insert("box_in_process", "В процессе");
  bMatchTable.insert("box_quantity", "Емкость бокса");

  bMatchTable.insert("box_assembled_units", "Собрано транспондеров");
  bMatchTable.insert("box_assembling_start", "Начало сборки");
  bMatchTable.insert("box_assembling_end", "Конец сборки");

  bMatchTable.insert("first_transponder_sn", "Первый транспондер");
  bMatchTable.insert("last_transponder_sn", "Последний транспондер");

  bMatchTable.insert("pallet_id", "ID паллеты");
  bMatchTable.insert("production_line_id", "ID производственной линии");

  BoxModel.setMatchTable(bMatchTable);
}
