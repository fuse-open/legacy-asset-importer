file(GLOB SRC
    ../../3rdparty/ConvertUTF/ConvertUTF.h
    ../../3rdparty/ConvertUTF/ConvertUTF.c
    ../../include/Xli/*.h
    ../../include/Xli.h
    *.cpp)

if (WIN32)
    file(GLOB MORE_SRC win32/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})
else()
    file(GLOB MORE_SRC pthread/*.cpp unix/*.cpp)
    set(SRC ${SRC} ${MORE_SRC})

    if (APPLE)
        file(GLOB MORE_SRC apple/*.mm)
        set(SRC ${SRC} ${MORE_SRC})
    elseif (ANDROID)
        file(GLOB MORE_SRC android/*.cpp)
        set(SRC ${SRC} ${MORE_SRC})
        set(LIBS ${LIBS} android log)
    else()
        file(GLOB MORE_SRC linux/*.cpp)
        set(SRC ${SRC} ${MORE_SRC})
        set(LIBS ${LIBS} pthread)
    endif()
endif()

add_library(Xli ${SRC})
target_link_libraries(Xli ${LIBS})
