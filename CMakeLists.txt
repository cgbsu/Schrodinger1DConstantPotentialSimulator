cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME Schrodinger1DConstantPotentialSimulator)
project(Schrodinger1DConstantPotentialSimulator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(MAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH ${QTDIR})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core         REQUIRED)
find_package(Qt5Gui          REQUIRED)
find_package(Qt5Widgets      REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Charts       REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/Source/*.cpp ${HeaderFiles})

qt5_add_resources(SOURCE_FILES resource.qrc)

add_compile_definitions(QT_DEPRECATED_WARNINGS)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Gui Qt5::Core Qt5::PrintSupport Qt5::Charts)

