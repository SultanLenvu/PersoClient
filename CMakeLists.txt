cmake_minimum_required(VERSION 3.16)
project(PersoClient VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PersoClient WIN32 MACOSX_BUNDLE
    Environment/client_manager.cpp Environment/client_manager.h
    Environment/perso_client.cpp Environment/perso_client.h
    GUI/abstract_gui.cpp GUI/abstract_gui.h
    GUI/authorization_dialog.cpp GUI/authorization_dialog.h
    GUI/authorization_gui.cpp GUI/authorization_gui.h
    GUI/custom_transponder_sticker_dialog.cpp GUI/custom_transponder_sticker_dialog.h
    GUI/input_dialog.cpp GUI/input_dialog.h
    GUI/interaction_system.cpp GUI/interaction_system.h
    GUI/mainwindow_kernel.cpp GUI/mainwindow_kernel.h
    GUI/master_gui.cpp GUI/master_gui.h
    GUI/master_password_input_dialog.cpp GUI/master_password_input_dialog.h
    GUI/production_gui.cpp GUI/production_gui.h
    GUI/testing_gui.cpp GUI/testing_gui.h
    GUI/transponder_sticker_scan_dialog.cpp GUI/transponder_sticker_scan_dialog.h
    General/definitions.h
    General/hash_model.cpp General/hash_model.h
    General/types.h
    Log/file_log_backend.cpp Log/file_log_backend.h
    Log/log_backend.cpp Log/log_backend.h
    Log/log_system.cpp Log/log_system.h
    Log/widget_log_backend.cpp Log/widget_log_backend.h
    Programmers/interface_programmer.cpp Programmers/interface_programmer.h
    Programmers/jlink_exe_programmer.cpp Programmers/jlink_exe_programmer.h
    StickerPrinter/isticker_printer.cpp StickerPrinter/isticker_printer.h
    StickerPrinter/te310_printer.cpp StickerPrinter/te310_printer.h
    main.cpp
)
target_link_libraries(PersoClient PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(PersoClient PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS PersoClient
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET PersoClient
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
