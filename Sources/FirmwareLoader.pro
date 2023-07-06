QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Environment/load_manager.cpp \
    Environment/log_system.cpp \
    Environment/notification_system.cpp \
    GUI/mainwindow_gui.cpp \
    Programmers/interface_programmer.cpp \
    Programmers/jlink_manual_programmer.cpp \
    main.cpp \
    GUI/mainwindow.cpp

HEADERS += \
    Environment/definitions.h \
    Environment/load_manager.h \
    Environment/log_system.h \
    Environment/notification_system.h \
    GUI/mainwindow.h \
    GUI/mainwindow_gui.h \
    Programmers/interface_programmer.h \
    Programmers/jlink_manual_programmer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
