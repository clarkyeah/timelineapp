cmake_minimum_required(VERSION 3.5)

project(TimelineApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    timelinewidget.cpp
    timelinewidget.h
)

qt6_wrap_cpp(PROJECT_SOURCES_MOC ${PROJECT_SOURCES})

qt_add_executable(TimelineApp
    ${PROJECT_SOURCES}
    ${PROJECT_SOURCES_MOC}
)

target_link_libraries(TimelineApp PRIVATE Qt6::Core Qt6::Widgets)
