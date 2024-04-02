#include "programmer_gui_subkernel.h"
#include "async_programmer.h"
#include "global_environment.h"

#include <QFileDialog>
#include <QMessageBox>

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {}


void ProgrammerGuiSubkernel::programMemory() {
  emit clearLogDisplay_signal();

  QString path(QFileDialog::getOpenFileName(nullptr, "Выберите файл", "",
                                            "Все файлы (*.*)"));

  // Считываем данные из файла
  QFile tempFile(path);
  if (!tempFile.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(nullptr, "Ошибка",
                          "Не удалось открыть считать содержимое файла.",
                          QMessageBox::Ok);
    return;
  }

  QByteArray data;
  data.reserve(tempFile.size());
  data = tempFile.readAll();

  emit programMemory_signal(data);
}

void ProgrammerGuiSubkernel::readMemory() {
  emit clearLogDisplay_signal();

  emit readMemory_signal();
}

void ProgrammerGuiSubkernel::eraseMemory() {
  emit clearLogDisplay_signal();

  emit eraseMemory_signal();
}

void ProgrammerGuiSubkernel::programUserData() {
  emit clearLogDisplay_signal();

  QString path(QFileDialog::getOpenFileName(nullptr, "Выберите файл", "",
                                            "Все файлы (*.*)"));

  // Считываем данные из файла
  QFile tempFile(path);
  if (!tempFile.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(nullptr, "Ошибка",
                          "Не удалось открыть считать содержимое файла.",
                          QMessageBox::Ok);
    return;
  }

  QByteArray data;
  data.reserve(tempFile.size());
  data = tempFile.readAll();

  emit programUserData_signal(data);
}

void ProgrammerGuiSubkernel::readUserData() {
  emit clearLogDisplay_signal();

  emit readUserData_signal();
}

void ProgrammerGuiSubkernel::readUcid() {
  emit readUcid_signal();

  emit clearLogDisplay_signal();
}

void ProgrammerGuiSubkernel::unlockMemory() {
  emit clearLogDisplay_signal();

  emit unlockMemory_signal();
}

void ProgrammerGuiSubkernel::lockMemory() {
  emit clearLogDisplay_signal();

  emit lockMemory_signal();
}

void ProgrammerGuiSubkernel::connectDependencies() const {
  AsyncProgrammer* ap =
      GlobalEnvironment::instance()->getObject<AsyncProgrammer>(
          "AsyncProgrammer");

  connect(this, &ProgrammerGuiSubkernel::programMemory_signal, ap,
          &AsyncProgrammer::programMemory);
  connect(this, &ProgrammerGuiSubkernel::readMemory_signal, ap,
          &AsyncProgrammer::readMemory);
  connect(this, &ProgrammerGuiSubkernel::eraseMemory_signal, ap,
          &AsyncProgrammer::eraseMemory);

  connect(this, &ProgrammerGuiSubkernel::programUserData_signal, ap,
          &AsyncProgrammer::programUserData);
  connect(this, &ProgrammerGuiSubkernel::readUserData_signal, ap,
          &AsyncProgrammer::readUserData);
  connect(this, &ProgrammerGuiSubkernel::readUcid_signal, ap,
          &AsyncProgrammer::readTransponderUcid);

  connect(this, &ProgrammerGuiSubkernel::unlockMemory_signal, ap,
          &AsyncProgrammer::unlockMemory);
  connect(this, &ProgrammerGuiSubkernel::lockMemory_signal, ap,
          &AsyncProgrammer::lockMemory);
}
