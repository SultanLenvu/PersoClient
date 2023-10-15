QT += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#LIBS += "C:\Workspace\Development\QT_Sample\TSCLIB.dll"

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/interaction_system.cpp \
    General/map_model.cpp \
    Log/log_system.cpp \
    Log/log_backend.cpp \
    Log/text_stream_log_backend.cpp \
    Log/widget_log_backend.cpp \
    Environment/client_manager.cpp \
    Environment/perso_client.cpp \
    GUI/authorization_dialog.cpp \
    GUI/custom_transponder_sticker_dialog.cpp \
    GUI/transponder_sticker_scan_dialog.cpp \
    GUI/authorization_gui.cpp \
    GUI/gui.cpp \
    GUI/mainwindow_kernel.cpp \
    GUI/master_gui.cpp \
    GUI/production_gui.cpp \
    Programmers/interface_programmer.cpp \
    Programmers/jlink_exe_programmer.cpp \
    StickerPrinter/isticker_printer.cpp \
    StickerPrinter/te310_printer.cpp \
    main.cpp

HEADERS += \
    GUI/interaction_system.h \
    General/map_model.h \
    Log/log_system.h \
    Log/log_backend.h \
    Log/text_stream_log_backend.h \
    Log/widget_log_backend.h \
    Environment/client_manager.h \
    Environment/perso_client.h \
    GUI/authorization_dialog.h \
    GUI/custom_transponder_sticker_dialog.h \
    GUI/transponder_sticker_scan_dialog.h \
    GUI/authorization_gui.h \
    GUI/gui.h \
    GUI/mainwindow_kernel.h \
    GUI/master_gui.h \
    GUI/production_gui.h \
    Programmers/interface_programmer.h \
    Programmers/jlink_exe_programmer.h \
    StickerPrinter/isticker_printer.h \
    StickerPrinter/te310_printer.h \
    General/types.h \
    General/definitions.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
