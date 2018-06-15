file(GLOB SRC
    ../../include/XliJson/*.h
    ../../include/XliJson.h
    ../../3rdparty/JSON_parser/*.h
    ../../3rdparty/JSON_parser/*.c
    *.cpp)

set(LIBS Xli)

add_library(XliJson ${SRC})
target_link_libraries(XliJson ${LIBS})
