cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME Schrodinger1DConstantPotentialSimulator)
project(Schrodinger1DConstantPotentialSimulator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(MAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

include(FetchContent) # If not included already

FetchContent_Declare(sciplot_content
  GIT_REPOSITORY https://github.com/sciplot/sciplot.git
  GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
  FetchContent_Populate(sciplot_content)
endif()

include_directories(${sciplot_content_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/Include)
FILE(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/Source/*.cpp ${HeaderFiles})

include(FetchContent)

FetchContent_Declare(
		matplotpp
		GIT_REPOSITORY https://github.com/nlamprian/matplotpp.git
		GIT_TAG master
	)
FetchContent_GetProperties(matplotpp)
if(NOT matplotpp_content_POPULATED)
  FetchContent_Populate(matplotpp)
endif()

include_directories(${matplotpp_content_SOURCE_DIR})

find_package(matplotpp)

add_compile_definitions(QT_DEPRECATED_WARNINGS)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} matplotpp::matplotpp)

