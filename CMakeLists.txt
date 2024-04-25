cmake_minimum_required(VERSION 3.16)
project(PersoClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QT
#======================================================================================================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui Widgets Network PrintSupport)
#======================================================================================================#

# Получение пути к директории src
get_filename_component(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" ABSOLUTE)

# Получение списка файлов в директории src и её поддиректориях
file(GLOB_RECURSE SOURCE_FILES "${SRC_DIR}/*")

# Создаем цель сборки, исполняемый файл
add_executable(PersoClient ${SOURCE_FILES})

# Включаем поиск по заголовочных файлов директориям
target_include_directories(PersoClient PRIVATE "${SRC_DIR}")

# Связываем цель сборки с динамическими библиотеками
target_link_libraries(PersoClient PRIVATE
    Qt::Widgets
    Qt::Core
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
)

# Размещение собранной цели в заданной директории
install(TARGETS PersoClient
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Размещение файлов конфигурации в UNIX-системе
if (UNIX AND NOT APPLE)
    install(FILES supplemental/persoclient.desktop DESTINATION ${DESTDIR}/usr/share/applications)
endif (UNIX AND NOT APPLE)

# Сборка мануала для пользователей
add_custom_target(manual pandoc -s --pdf-engine=xelatex manual.md -V "mainfont:PT Sans" -V "lang:ru" -V papersize:a4 -V "geometry:margin=0.5in" -V "mainfontoptions:Scale=1.5" -o manual.pdf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc/manual
)
