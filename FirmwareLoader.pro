QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Environment/log_system.cpp \
    Environment/programmator_manager.cpp \
    Environment/user_Interaction_system.cpp \
    GUI/gui.cpp \
    GUI/master_gui.cpp \
    GUI/production_gui.cpp \
    Programmers/interface_programmer.cpp \
    Programmers/jlink_exe_programmer.cpp \
    main.cpp \
    GUI/mainwindow.cpp

HEADERS += \
    Environment/definitions.h \
    Environment/log_system.h \
    Environment/programmator_manager.h \
    Environment/user_Interaction_system.h \
    GUI/gui.h \
    GUI/mainwindow.h \
    GUI/master_gui.h \
    GUI/production_gui.h \
    Programmers/interface_programmer.h \
    Programmers/jlink_exe_programmer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
