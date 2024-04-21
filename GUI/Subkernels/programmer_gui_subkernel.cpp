#include "programmer_gui_subkernel.h"

#include <QFileDialog>
#include <QMessageBox>

ProgrammerGuiSubkernel ::ProgrammerGuiSubkernel(const QString& name)
    : AbstractGuiSubkernel{name} {
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
