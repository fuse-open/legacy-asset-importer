project(PunchTest)

cmake_minimum_required(VERSION 2.8)

set(XLI_DIR ${PROJECT_SOURCE_DIR}/../../3rdparty/Xli CACHE PATH "Path to Xli")
include(${XLI_DIR}/Xli.cmake)

link_project(${PROJECT_SOURCE_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(TestFont TestFont.cpp)
target_link_libraries(TestFont Punch)

add_executable(FontFaceMT FontFaceMT.cpp)
target_link_libraries(FontFaceMT Punch)
