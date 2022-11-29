cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME Schrodinger1DConstantPotentialSimulator)
project(Schrodinger1DConstantPotentialSimulator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(MAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

#find_package(imgui CONFIG)
#find_package(glfw3 CONFIG)
#find_package(glew CONFIG)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/Source/*.cpp ${HeaderFiles})

include(FetchContent)

add_compile_definitions(QT_DEPRECATED_WARNINGS)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

