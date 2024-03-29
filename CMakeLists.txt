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
include_directories(Management/Managers)
include_directories(Management/AsyncWrappers)

include_directories(GUI/)
include_directories(GUI/Dialogs)
include_directories(GUI/Subkernels)
include_directories(GUI/Reactions)
include_directories(GUI/Widgets)

include_directories(Service/)
include_directories(Service/Functors)
include_directories(Service/General)
include_directories(Service/Log)
include_directories(Service/ObjectTypes)

include_directories(Programmers/)
include_directories(StickerPrinter/)

include_directories(ServerConnection/)
include_directories(ServerConnection/Commands/)
#======================================================================================================#

add_executable(PersoClient WIN32
    Service/abstract_service_object_space.h Service/abstract_service_object_space.cpp
    Service/service_object_space.h Service/service_object_space.cpp
    Service/asynchronous_object_space.h Service/asynchronous_object_space.cpp
    Service/configuration_manager.h Service/configuration_manager.cpp

    Service/General/definitions.h
    Service/General/hash_table_model.cpp Service/General/hash_table_model.h
    Service/General/types.h
    Service/General/global_environment.cpp Service/General/global_environment.h
    Service/General/named_object_factory.h Service/General/named_object_factory.cpp

    Service/Functors/abstract_string_checker.h Service/Functors/abstract_string_checker.cpp
    Service/Functors/ucid_checker.h Service/Functors/ucid_checker.cpp
    Service/Functors/numeric_string_checker.h Service/Functors/numeric_string_checker.cpp

    Service/ObjectTypes/named_object.cpp Service/ObjectTypes/named_object.h
    Service/ObjectTypes/configurable_object.h Service/ObjectTypes/configurable_object.cpp
    Service/ObjectTypes/loggable_object.h Service/ObjectTypes/loggable_object.cpp
    Service/ObjectTypes/log_system_bundle.h Service/ObjectTypes/log_system_bundle.cpp

    Service/Log/file_log_backend.cpp Service/Log/file_log_backend.h
    Service/Log/abstract_log_backend.cpp Service/Log/abstract_log_backend.h
    Service/Log/log_system.cpp Service/Log/log_system.h
    Service/Log/widget_log_backend.cpp Service/Log/widget_log_backend.h

    Management/Managers/abstract_manager.h Management/Managers/abstract_manager.cpp
    Management/Managers/production_manager.h Management/Managers/production_manager.cpp
    Management/Managers/authorization_manager.h Management/Managers/authorization_manager.cpp

    Management/AsyncWrappers/i_async_wrapper.h Management/AsyncWrappers/i_async_wrapper.cpp
    Management/AsyncWrappers/programmer_async_wrapper.cpp Management/AsyncWrappers/programmer_async_wrapper.h
    Management/AsyncWrappers/progressable_async_wrapper.cpp Management/AsyncWrappers/progressable_async_wrapper.h
    Management/AsyncWrappers/server_connection_async_wrapper.cpp Management/AsyncWrappers/server_connection_async_wrapper.h
    Management/AsyncWrappers/sticker_printer_async_wrapper.cpp Management/AsyncWrappers/sticker_printer_async_wrapper.h
    Management/AsyncWrappers/production_line_manager_async_wrapper.cpp Management/AsyncWrappers/production_line_manager_async_wrapper.h
    Management/AsyncWrappers/async_wrapper.h Management/AsyncWrappers/async_wrapper.cpp

    GUI/gui_kernel.cpp GUI/gui_kernel.h

    GUI/Widgets/authorization_gui.cpp GUI/Widgets/authorization_gui.h
    GUI/Widgets/master_gui.cpp GUI/Widgets/master_gui.h
    GUI/Widgets/production_assembler_gui.cpp GUI/Widgets/production_assembler_gui.h
    GUI/Widgets/production_tester_gui.cpp GUI/Widgets/production_tester_gui.h

    GUI/Subkernels/abstract_gui_subkernel.h GUI/Subkernels/abstract_gui_subkernel.cpp
    GUI/Subkernels/production_gui_subkernel.h GUI/Subkernels/production_gui_subkernel.cpp
    GUI/Subkernels/programmer_gui_subkernel.h GUI/Subkernels/programmer_gui_subkernel.cpp
    GUI/Subkernels/sticker_printer_gui_subkernel.h GUI/Subkernels/sticker_printer_gui_subkernel.cpp
    GUI/Subkernels/assembly_unit_gui_subkernel.cpp GUI/Subkernels/assembly_unit_gui_subkernel.h
    GUI/Subkernels/server_connection_gui_subkernel.cpp GUI/Subkernels/server_connection_gui_subkernel.h
    GUI/Subkernels/authorization_gui_subkernel.h GUI/Subkernels/authorization_gui_subkernel.cpp

    GUI/Dialogs/authorization_dialog.cpp GUI/Dialogs/authorization_dialog.h
    GUI/Dialogs/master_password_input_dialog.cpp GUI/Dialogs/master_password_input_dialog.h
    GUI/Dialogs/custom_transponder_sticker_dialog.cpp GUI/Dialogs/custom_transponder_sticker_dialog.h
    GUI/Dialogs/transponder_sticker_scan_dialog.cpp GUI/Dialogs/transponder_sticker_scan_dialog.h
    GUI/Dialogs/abstract_input_dialog.cpp GUI/Dialogs/abstract_input_dialog.h
    GUI/Dialogs/settings_dialog.cpp GUI/Dialogs/settings_dialog.h

    GUI/Reactions/i_progress_indicator.h GUI/Reactions/i_progress_indicator.cpp
    GUI/Reactions/progress_indicator.cpp GUI/Reactions/progress_indicator.h
    GUI/Reactions/i_status_indicator.h GUI/Reactions/i_status_indicator.cpp
    GUI/Reactions/status_indicator.h GUI/Reactions/status_indicator.cpp

    Programmers/i_programmer.cpp Programmers/i_programmer.h
    Programmers/jlink_exe_programmer.cpp Programmers/jlink_exe_programmer.h
    Programmers/transponder_firmware.h Programmers/transponder_firmware.cpp
    Programmers/transponder_user_data.h Programmers/transponder_user_data.cpp

    StickerPrinter/i_sticker_printer.cpp StickerPrinter/i_sticker_printer.h
    StickerPrinter/te310_printer.cpp StickerPrinter/te310_printer.h

    ServerConnection/perso_server_connection.cpp ServerConnection/perso_server_connection.h
    ServerConnection/i_server_connection.h ServerConnection/i_server_connection.cpp
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

add_custom_target(manual pandoc -s --pdf-engine=xelatex manual.md -V "mainfont:PT Sans" -V "lang:ru" -V papersize:a4 -V "geometry:margin=0.5in" -V "mainfontoptions:Scale=1.5" -o manual.pdf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Doc/manual
)

if (UNIX AND NOT APPLE)
    install(FILES supplemental/persoclient.desktop DESTINATION ${DESTDIR}/usr/share/applications)
endif (UNIX AND NOT APPLE)
