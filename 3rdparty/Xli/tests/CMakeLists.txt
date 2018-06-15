project("Xli Tests")

cmake_minimum_required(VERSION 2.6)

include(${PROJECT_SOURCE_DIR}/../Xli.cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

file(GLOB SRC
    Xli/*.cpp 
    *.cpp)

add_executable(XliTest ${SRC})
target_link_libraries(XliTest Xli)
