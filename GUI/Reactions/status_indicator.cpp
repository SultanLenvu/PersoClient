#include <QMessageBox>

#include "status_indicator.h"

StatusIndicator::StatusIndicator(const QString& name)
    : NamedObject{name} {
  createMessageMatchTable();
}

void StatusIndicator::handle(ReturnStatus ret) {
  if (ret == ReturnStatus::NoError) {
    QMessageBox::information(nullptr, "Сообщение", MessageTable[ret],
                             QMessageBox::Ok);
    return;
  }
  if (MessageTable.count(ret) == 0) {
    QMessageBox::critical(nullptr, "Ошибка",
                          "Получен неизвестный статус возврата.",
                          QMessageBox::Ok);
    return;
  }

  QMessageBox::critical(nullptr, "Ошибка", MessageTable[ret], QMessageBox::Ok);
}

void StatusIndicator::createMessageMatchTable() {
  MessageTable[ReturnStatus::NoError] = "Выполнено.";
  MessageTable[ReturnStatus::ParameterError] = "Получена ошибка параметра.";
  MessageTable[ReturnStatus::SyntaxError] = "Получена синтаксическая ошибка.";
  MessageTable[ReturnStatus::ConsistencyViolation] =
      "Получена ошибка синхронизации.";
  MessageTable[ReturnStatus::FileOpenError] =
      "Получена ошибка при открытии файла.";
  MessageTable[ReturnStatus::InvalidFirmwareFile] =
      "Получен некорректный файл прошивки.";

  MessageTable[ReturnStatus::ServerConnectionError] =
      "Не удалось подключиться к серверу.";
  MessageTable[ReturnStatus::ServerConnectionMissed] =
      "Отсутствует подключение к серверу.";
  MessageTable[ReturnStatus::ServerNotResponding] = "Сервер не отвечает.";
  MessageTable[ReturnStatus::ServerDataTransmittingError] =
      "Получена ошибка при отправке данных.";
  MessageTable[ReturnStatus::ServerResponseSyntaxError] =
      "Получена синтаксическая ошибка при обработке ответа от сервера.";
  MessageTable[ReturnStatus::ServerResponseDataBlockError] =
      "Получена ошибка обработке блока данных от сервера.";
  MessageTable[ReturnStatus::ServerResponseProcessingError] =
      "Получена ошибка при обработке ответа от сервера.";

  MessageTable[ReturnStatus::ClientCommandParamError] =
      "Получена ошибка в параметре клиентской комаенды.";
  MessageTable[ReturnStatus::ServerInternalError] =
      "Получена серверная ошибка.";

  MessageTable[ReturnStatus::ProductionContextNotValid] =
      "Производство не запущено.";

  MessageTable[ReturnStatus::ProductionLineMissed] =
      "Производственная линия не найдена.";
  MessageTable[ReturnStatus::TransponderMissed] = "Транспондер не найден.";
  MessageTable[ReturnStatus::BoxMissed] = "Бокс не найден.";
  MessageTable[ReturnStatus::PalletMissed] = "Паллета не найдена.";
  MessageTable[ReturnStatus::OrderMissed] = "Заказ не найден.";
  MessageTable[ReturnStatus::IssuerMissed] = "Заказчик не найден.";
  MessageTable[ReturnStatus::MasterKeysMissed] =
      "Ключи безопасности не найдены.";

  MessageTable[ReturnStatus::OrderMultiplyAssembly] =
      "Несколько заказов одновременно находятся в процессе сборки.";
  MessageTable[ReturnStatus::OrderInProcessMissed] =
      "Отсутствуют заказы в процессе сборки.";
  MessageTable[ReturnStatus::OrderCompletelyAssembled] =
      "Сборка текущего заказа завершена.";

  MessageTable[ReturnStatus::BoxAlreadyRequested] =
      "Бокс был получен ранее и находится в процессе сборки.";
  MessageTable[ReturnStatus::FreeBoxMissed] =
      "Не найден свободный бокс для сборки.";
  MessageTable[ReturnStatus::BoxIsEmty] = "В боксе отсутствуют транспондеры.";
  MessageTable[ReturnStatus::BoxOverflow] = "Бокс переполнен.";
  MessageTable[ReturnStatus::BoxCompletelyAssembled] =
      "Все транспондеры в боксе собраны.";
  MessageTable[ReturnStatus::BoxNotCompletelyAssembled] =
      "В боксе собраны не все транспондеры.";

  MessageTable[ReturnStatus::PalletIsEmpty] = "В паллете отсутствуют боксы.";
  MessageTable[ReturnStatus::PalletOverflow] = "Паллета переполнена.";

  MessageTable[ReturnStatus::TransponderIncorrectRerelease] =
      "Данный транспондер не был выпущен ранее. Перевыпуск невозможен.";
  MessageTable[ReturnStatus::TransponderIdenticalUcidError] =
      "Данная печатная плата уже была использована при сборке. Используйте "
      "другую плату.";
  MessageTable[ReturnStatus::TransponderRollbackLimit] =
      "В текущем боксе нет собранных транспондеров. Откат невозможен.";

  MessageTable[ReturnStatus::ProductionLineContextNotAuthorized] =
      "Контекст производственной линии не авторизирован.";
  MessageTable[ReturnStatus::ProductionLineNotLaunched] =
      "Производственная линия не запущена.";
  MessageTable[ReturnStatus::ProductionLineLaunchError] =
      "Не удалось запустить производственную линию.";
  MessageTable[ReturnStatus::ProductionLineAlreadyLaunched] =
      "Производственная линия уже запущена.";
  MessageTable[ReturnStatus::ProductionLineAlreadyInProcess] =
      "Производственная линия уже находится в процессе сборки бокса.";
  MessageTable[ReturnStatus::ProductionLineNotActive] =
      "Производственная линия не активирована.";
  MessageTable[ReturnStatus::ProductionLineCompleted] =
      "Производственная линия завершила свою работу.";
  MessageTable[ReturnStatus::ProductionLineNotInProcess] =
      "Производственная линия не находится в процессе сборки. Необходимо "
      "запросить бокс.";

  MessageTable[ReturnStatus::StickerPrintingFailed] =
      "Не удалось распечататать стикер.";
  MessageTable[ReturnStatus::StickerPrinterDriverMissed] =
      "Не найден драйвер для принтера стикеров.";
  MessageTable[ReturnStatus::StickerPrinterLibraryMissing] =
      "Не найдена динамическая библиотека для принтера стикеров.";
  MessageTable[ReturnStatus::StickerPrinterInitError] =
      "Не удалось инициализировать принтер стикеров.";
  MessageTable[ReturnStatus::StickerPrinterConnectionError] =
      "Не удалось подключиться к принтеру стикеров.";
  MessageTable[ReturnStatus::StickerPrintError] =
      "Не удалось распечатать стикер.";
  MessageTable[ReturnStatus::LastStickerMissed] =
      "Ранее не было распечатано ни одного стикера. Повтор печати невозможен.";

  MessageTable[ReturnStatus::InvalidFirmwareFile] =
      "Получен некорректный файл прошивки.";
  MessageTable[ReturnStatus::ProgrammatorLibraryMissing] =
      "Не найдена библиотека программатора.";
  MessageTable[ReturnStatus::ProgrammatorExeFileInvalid] =
      "Получен некорректный исполняемый файл для работы с программатором.";
  MessageTable[ReturnStatus::ProgrammatorCommandScriptError] =
      "Получена ошибка программатора.";

  MessageTable[ReturnStatus::Unknown] = "Получена неизвестная ошибка.";
}
