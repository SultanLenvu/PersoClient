QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Environment/client_manager.cpp \
    Environment/log_system.cpp \
    Environment/perso_client.cpp \
    Environment/transponder_seed_model.cpp \
    GUI/authorization_dialog.cpp \
    GUI/user_interaction_system.cpp \
    GUI/authorization_gui.cpp \
    GUI/gui.cpp \
    GUI/mainwindow_kernel.cpp \
    GUI/master_gui.cpp \
    GUI/production_gui.cpp \
    GUI/transponder_sticker_dialog.cpp \
    Programmers/interface_programmer.cpp \
    Programmers/jlink_exe_programmer.cpp \
    main.cpp

HEADERS += \
    Environment/client_manager.h \
    Environment/log_system.h \
    Environment/perso_client.h \
    Environment/transponder_seed_model.h \
    GUI/authorization_dialog.h \
    GUI/user_interaction_system.h \
    GUI/authorization_gui.h \
    GUI/gui.h \
    GUI/mainwindow_kernel.h \
    GUI/master_gui.h \
    GUI/production_gui.h \
    GUI/transponder_sticker_dialog.h \
    General/types.h \
    General/definitions.h \
    Programmers/interface_programmer.h \
    Programmers/jlink_exe_programmer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
