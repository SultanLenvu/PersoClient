#include "programmer_gui_subkernel.h"
#include "async_programmer.h"
#include "global_environment.h"

#include <QFileDialog>
#include <QMessageBox>

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
  connectDependencies();
}

void ProgrammerGuiSubkernel::programMemory() {
  emit clearLogDisplay_signal();

  QString path(QFileDialog::getOpenFileName(nullptr, "Выберите файл", "",
                                            "Все файлы (*.*)"));

  if (path.isEmpty()) {
    return;
  }

  emit programMemory_signal(path);
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

  QString path(QFileDialog::getOpenFileName(
      nullptr, "Выберите файл", QDir::currentPath(), "Все файлы (*.*)"));

  if (path.isEmpty()) {
    return;
  }

  emit programUserData_signal(path);
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

void ProgrammerGuiSubkernel::displayUcid(const QString& ucid) {
  QMessageBox messageBox;
  messageBox.setWindowTitle("UCID");
  messageBox.setIcon(QMessageBox::Information);
  messageBox.setText(ucid);
  messageBox.setTextInteractionFlags(Qt::TextSelectableByMouse);

  messageBox.exec();
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

  connect(ap, &AsyncProgrammer::transponderUcidReady, this,
          &ProgrammerGuiSubkernel::displayUcid);
}
