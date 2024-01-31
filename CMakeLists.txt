cmake_minimum_required(VERSION 3.16)
project(PersoClient VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#========================================== INCLUSION OF QT ===========================================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network PrintSupport Core5Compat)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)
#======================================================================================================#

#========================================== SOURCE DIRECTORIES ===========================================#
include_directories(Management/)
include_directories(GUI/)
include_directories(GUI/Dialogs)
include_directories(GUI/Subkernels)
include_directories(General/)
include_directories(Log/)
include_directories(Programmers/)
include_directories(Printing/)
include_directories(ServerConnection/)
include_directories(ServerConnection/Commands/)
#======================================================================================================#

add_executable(PersoClient WIN32
    Management/global_environment.cpp Management/global_environment.h
    Management/abstract_manager.h Management/abstract_manager.cpp
    Management/production_manager.h Management/production_manager.cpp
    Management/programmer_manager.h Management/programmer_manager.cpp
    Management/sticker_printer_manager.h Management/sticker_printer_manager.cpp

    GUI/abstract_gui.cpp GUI/abstract_gui.h
    GUI/authorization_gui.cpp GUI/authorization_gui.h
    GUI/interaction_system.cpp GUI/interaction_system.h
    GUI/gui_kernel.cpp GUI/gui_kernel.h
    GUI/master_gui.cpp GUI/master_gui.h
    GUI/production_assembler_gui.cpp GUI/production_assembler_gui.h
    GUI/production_tester_gui.cpp GUI/production_tester_gui.h

    GUI/Subkernels/abstract_gui_subkernel.h GUI/Subkernels/abstract_gui_subkernel.cpp
    GUI/Subkernels/production_gui_subkernel.h GUI/Subkernels/production_gui_subkernel.cpp
    GUI/Subkernels/programmer_gui_subkernel.h GUI/Subkernels/programmer_gui_subkernel.cpp
    GUI/Subkernels/sticker_printer_gui_subkernel.h GUI/Subkernels/sticker_printer_gui_subkernel.cpp

    GUI/Dialogs/authorization_dialog.cpp GUI/Dialogs/authorization_dialog.h
    GUI/Dialogs/master_password_input_dialog.cpp GUI/Dialogs/master_password_input_dialog.h
    GUI/Dialogs/custom_transponder_sticker_dialog.cpp GUI/Dialogs/custom_transponder_sticker_dialog.h
    GUI/Dialogs/transponder_sticker_scan_dialog.cpp GUI/Dialogs/transponder_sticker_scan_dialog.h
    GUI/Dialogs/abstract_input_dialog.cpp GUI/Dialogs/abstract_input_dialog.h
    GUI/Dialogs/settings_dialog.cpp GUI/Dialogs/settings_dialog.h

    General/definitions.h
    General/hash_table_model.cpp General/hash_table_model.h
    General/types.h

    Log/file_log_backend.cpp Log/file_log_backend.h
    Log/log_backend.cpp Log/log_backend.h
    Log/log_system.cpp Log/log_system.h
    Log/widget_log_backend.cpp Log/widget_log_backend.h

    Programmers/abstract_programmer.cpp Programmers/abstract_programmer.h
    Programmers/jlink_exe_programmer.cpp Programmers/jlink_exe_programmer.h

    Printing/abstract_sticker_printer.cpp Printing/abstract_sticker_printer.h
    Printing/te310_printer.cpp Printing/te310_printer.h

    ServerConnection/perso_server_connection.cpp ServerConnection/perso_server_connection.h
    ServerConnection/abstract_server_connection.h ServerConnection/abstract_server_connection.cpp
    ServerConnection/Commands/abstract_client_command.h ServerConnection/Commands/abstract_client_command.cpp
    ServerConnection/Commands/echo.h ServerConnection/Commands/echo.cpp
    ServerConnection/Commands/release_transponder.h ServerConnection/Commands/release_transponder.cpp
    ServerConnection/Commands/confirm_transponder_release.h ServerConnection/Commands/confirm_transponder_release.cpp
    ServerConnection/Commands/rerelease_transponder.h ServerConnection/Commands/rerelease_transponder.cpp
    ServerConnection/Commands/confirm_transponder_rerelease.h ServerConnection/Commands/confirm_transponder_rerelease.cpp
    ServerConnection/Commands/rollback_transponder.h ServerConnection/Commands/rollback_transponder.cpp
    ServerConnection/Commands/print_box_sticker.h ServerConnection/Commands/print_box_sticker.cpp
    ServerConnection/Commands/print_last_box_sticker.h ServerConnection/Commands/print_last_box_sticker.cpp
    ServerConnection/Commands/print_pallet_sticker.h ServerConnection/Commands/print_pallet_sticker.cpp
    ServerConnection/Commands/print_last_pallet_sticker.h ServerConnection/Commands/print_last_pallet_sticker.cpp
    ServerConnection/Commands/launch_production_line.h ServerConnection/Commands/launch_production_line.cpp
    ServerConnection/Commands/shutdown_production_line.h ServerConnection/Commands/shutdown_production_line.cpp
    ServerConnection/Commands/get_current_transponder_data.h ServerConnection/Commands/get_current_transponder_data.cpp
    ServerConnection/Commands/get_transponder_data.cpp ServerConnection/Commands/get_transponder_data.h
    ServerConnection/Commands/refund_current_box.cpp ServerConnection/Commands/refund_current_box.h
    ServerConnection/Commands/get_current_box_data.cpp ServerConnection/Commands/get_current_box_data.h
    ServerConnection/Commands/complete_current_box.h ServerConnection/Commands/complete_current_box.cpp
    ServerConnection/Commands/request_box.h ServerConnection/Commands/request_box.cpp
    ServerConnection/Commands/get_production_line_data.h ServerConnection/Commands/get_production_line_data.cpp

    main.cpp
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

add_custom_target(docs doxygen doxygen.conf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

if (UNIX AND NOT APPLE)
    install(FILES supplemental/persoclient.desktop DESTINATION ${DESTDIR}/usr/share/applications)
endif (UNIX AND NOT APPLE)
