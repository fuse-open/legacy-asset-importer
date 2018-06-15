file(GLOB SRC
    ../../include/XliZip/*.h
    ../../include/XliZip.h
    GZip.cpp
    Zip.cpp)

set(LIBS Xli)

set(UNZIP_INC ../../3rdparty/unzip)

set(UNZIP_SRC
    ../../3rdparty/unzip/ioapi.c
    ../../3rdparty/unzip/unzip.c)

set(ZLIB_INC ../../3rdparty/zlib)

set(ZLIB_SRC
    ../../3rdparty/zlib/adler32.c
    ../../3rdparty/zlib/compress.c
    ../../3rdparty/zlib/crc32.c
    ../../3rdparty/zlib/deflate.c
    ../../3rdparty/zlib/infback.c
    ../../3rdparty/zlib/inffast.c
    ../../3rdparty/zlib/inflate.c
    ../../3rdparty/zlib/inftrees.c
    ../../3rdparty/zlib/trees.c
    ../../3rdparty/zlib/uncompr.c
    ../../3rdparty/zlib/zutil.c)

include_directories(${UNZIP_INC})
set(SRC ${SRC} ${UNZIP_SRC})

if (XLI_PLATFORM_WIN32 OR XLI_PLATFORM_IOS)

    include_directories(${ZLIB_INC})
    set(SRC ${SRC} ${ZLIB_SRC})

else()

    set(LIBS ${LIBS} z)

endif()

add_library(XliZip ${SRC})
target_link_libraries(XliZip ${LIBS})
