cmake_minimum_required(VERSION 3.16)
project(PersoClient VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network PrintSupport Core5Compat)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()

include_directories(Management/)
include_directories(GUI/)
include_directories(GUI/Dialogs)
include_directories(General/)
include_directories(Log/)
include_directories(Programmers/)
include_directories(Printing/)
include_directories(ClientConnection/)

qt_add_executable(PersoClient WIN32
    Management/client_manager.cpp Management/client_manager.h
    Management/perso_client.cpp Management/perso_client.h
    Management/global_environment.cpp Management/global_environment.h

    GUI/abstract_gui.cpp GUI/abstract_gui.h
    GUI/authorization_dialog.cpp GUI/authorization_dialog.h
    GUI/authorization_gui.cpp GUI/authorization_gui.h
    GUI/interaction_system.cpp GUI/interaction_system.h
    GUI/gui_kernel.cpp GUI/gui_kernel.h
    GUI/master_gui.cpp GUI/master_gui.h
    GUI/production_gui.cpp GUI/production_gui.h
    GUI/testing_gui.cpp GUI/testing_gui.h

    GUI/Subkernels/abstract_gui_subkernel.h GUI/Subkernels/abstract_gui_subkernel.cpp
    GUI/Subkernels/network_gui_subkernel.h GUI/Subkernels/network_gui_subkernel.cpp
    GUI/Subkernels/programmer_gui_subkernel.h GUI/Subkernels/programmer_gui_subkernel.cpp
    GUI/Subkernels/sticker_printer_gui_subkernel.h GUI/Subkernels/sticker_printer_gui_subkernel.cpp

    GUI/Dialogs/master_password_input_dialog.cpp GUI/Dialogs/master_password_input_dialog.h
    GUI/Dialogs/custom_transponder_sticker_dialog.cpp GUI/Dialogs/custom_transponder_sticker_dialog.h
    GUI/Dialogs/transponder_sticker_scan_dialog.cpp GUI/Dialogs/transponder_sticker_scan_dialog.h
    GUI/Dialogs/abstract_input_dialog.cpp GUI/Dialogs/abstract_input_dialog.h

    General/definitions.h
    General/hash_model.cpp General/hash_model.h
    General/types.h

    Log/file_log_backend.cpp Log/file_log_backend.h
    Log/log_backend.cpp Log/log_backend.h
    Log/log_system.cpp Log/log_system.h
    Log/widget_log_backend.cpp Log/widget_log_backend.h

    Programmers/interface_programmer.cpp Programmers/interface_programmer.h
    Programmers/jlink_exe_programmer.cpp Programmers/jlink_exe_programmer.h

    Printing/abstract_sticker_printer.cpp Printing/abstract_sticker_printer.h
    Printing/te310_printer.cpp Printing/te310_printer.h

    ClientConnection/abstract_client_connection.h ClientConnection/abstract_client_connection.cpp
    ClientConnection/client_connection.cpp ClientConnection/client_connection.h
    ClientConnection/abstract_client_command.h ClientConnection/abstract_client_command.cpp
    ClientConnection/echo_comand.h ClientConnection/echo_comand.cpp
    ClientConnection/release_command.h ClientConnection/release_command.cpp
    ClientConnection/release_confirm_command.h ClientConnection/release_confirm_command.cpp
    ClientConnection/rerelease_command.h ClientConnection/rerelease_command.cpp
    ClientConnection/rerelease_confirm_command.h ClientConnection/rerelease_confirm_command.cpp
    ClientConnection/rollback_command.h ClientConnection/rollback_command.cpp
    ClientConnection/box_sticker_print_command.h ClientConnection/box_sticker_print_command.cpp
    ClientConnection/last_box_sticker_print_command.h ClientConnection/last_box_sticker_print_command.cpp
    ClientConnection/pallet_sticker_print_command.h ClientConnection/pallet_sticker_print_command.cpp
    ClientConnection/last_pallet_sticker_print_command.h ClientConnection/last_pallet_sticker_print_command.cpp
    ClientConnection/log_in_command.h ClientConnection/log_in_command.cpp
    ClientConnection/log_out_command.h ClientConnection/log_out_command.cpp
    ClientConnection/update_command.h ClientConnection/update_command.cpp

    main.cpp
    Management/abstract_manager.h Management/abstract_manager.cpp
)
target_link_libraries(PersoClient PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt6::Core5Compat
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

add_custom_target(docs doxygen doxygen.conf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

