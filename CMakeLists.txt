cmake_minimum_required(VERSION 3.16)
project(MusicPlaylistManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(MusicPlaylistManager
    musictrack.cpp
    gui/main_qt.cpp
    gui/mainwindow.cpp
    gui/addsongdialog.cpp
    gui/selecttwodialog.cpp
)

target_include_directories(MusicPlaylistManager PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(MusicPlaylistManager PRIVATE Qt6::Widgets)
