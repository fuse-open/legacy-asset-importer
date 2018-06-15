file(GLOB SRC
    ../../include/XliImage/*.h
    ../../include/XliImage.h
    Bitmap.cpp
    CompressedImage.cpp
    Format.cpp
    ImageReader.cpp
    ImageWriter.cpp
    JpegReader.cpp
    JpegWriter.cpp
    Ktx.cpp
    PngReader.cpp
    PngWriter.cpp
    Texture.cpp)

set(LIBS Xli)

set(JPEG_INC ../../3rdparty/jpeg)

set(JPEG_SRC
    ../../3rdparty/jpeg/jcapimin.c
    ../../3rdparty/jpeg/jcapistd.c
    ../../3rdparty/jpeg/jccoefct.c
    ../../3rdparty/jpeg/jccolor.c
    ../../3rdparty/jpeg/jcdctmgr.c
    ../../3rdparty/jpeg/jchuff.c
    ../../3rdparty/jpeg/jcinit.c
    ../../3rdparty/jpeg/jcmainct.c
    ../../3rdparty/jpeg/jcmarker.c
    ../../3rdparty/jpeg/jcmaster.c
    ../../3rdparty/jpeg/jcomapi.c
    ../../3rdparty/jpeg/jcparam.c
    ../../3rdparty/jpeg/jcphuff.c
    ../../3rdparty/jpeg/jcprepct.c
    ../../3rdparty/jpeg/jcsample.c
    ../../3rdparty/jpeg/jctrans.c
    ../../3rdparty/jpeg/jdapimin.c
    ../../3rdparty/jpeg/jdapistd.c
    ../../3rdparty/jpeg/jdatadst.c
    ../../3rdparty/jpeg/jdatasrc.c
    ../../3rdparty/jpeg/jdcoefct.c
    ../../3rdparty/jpeg/jdcolor.c
    ../../3rdparty/jpeg/jddctmgr.c
    ../../3rdparty/jpeg/jdhuff.c
    ../../3rdparty/jpeg/jdinput.c
    ../../3rdparty/jpeg/jdmainct.c
    ../../3rdparty/jpeg/jdmarker.c
    ../../3rdparty/jpeg/jdmaster.c
    ../../3rdparty/jpeg/jdmerge.c
    ../../3rdparty/jpeg/jdphuff.c
    ../../3rdparty/jpeg/jdpostct.c
    ../../3rdparty/jpeg/jdsample.c
    ../../3rdparty/jpeg/jdtrans.c
    ../../3rdparty/jpeg/jerror.c
    ../../3rdparty/jpeg/jfdctflt.c
    ../../3rdparty/jpeg/jfdctfst.c
    ../../3rdparty/jpeg/jfdctint.c
    ../../3rdparty/jpeg/jidctflt.c
    ../../3rdparty/jpeg/jidctfst.c
    ../../3rdparty/jpeg/jidctint.c
    ../../3rdparty/jpeg/jidctred.c
    ../../3rdparty/jpeg/jmemmgr.c
    ../../3rdparty/jpeg/jmemnobs.c
    ../../3rdparty/jpeg/jquant1.c
    ../../3rdparty/jpeg/jquant2.c
    ../../3rdparty/jpeg/jutils.c)

set(PNG_INC ../../3rdparty/png)

set(PNG_SRC
    ../../3rdparty/png/png.c
    ../../3rdparty/png/pngerror.c
    ../../3rdparty/png/pngget.c
    ../../3rdparty/png/pngmem.c
    ../../3rdparty/png/pngpread.c
    ../../3rdparty/png/pngread.c
    ../../3rdparty/png/pngrio.c
    ../../3rdparty/png/pngrtran.c
    ../../3rdparty/png/pngrutil.c
    ../../3rdparty/png/pngset.c
    ../../3rdparty/png/pngtrans.c
    ../../3rdparty/png/pngwio.c
    ../../3rdparty/png/pngwrite.c
    ../../3rdparty/png/pngwtran.c
    ../../3rdparty/png/pngwutil.c)

if (XLI_PLATFORM_ANDROID)

    include_directories(${JPEG_INC} ${PNG_INC})
    set(SRC ${SRC} ${JPEG_SRC} ${PNG_SRC})
    set(LIBS ${LIBS} z)

elseif (XLI_PLATFORM_WIN32)

    include_directories(${JPEG_INC} ${PNG_INC} ../../3rdparty/zlib)
    set(SRC ${SRC} ${JPEG_SRC} ${PNG_SRC})
    set(LIBS ${LIBS} XliZip)

elseif (XLI_PLATFORM_IOS)
 
    include_directories(${JPEG_INC} ${PNG_INC})
    set(SRC ${SRC} ${JPEG_SRC} ${PNG_SRC})
    set(LIBS ${LIBS} XliZip)

elseif (XLI_PLATFORM_OSX)

    include_directories(${JPEG_INC} ${PNG_INC})
    set(SRC ${SRC} ${JPEG_SRC} ${PNG_SRC})
    set(LIBS ${LIBS} z)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,CoreFoundation -Wl,-framework,CoreServices -Wl,-framework,ApplicationServices")

elseif (XLI_PLATFORM_LINUX)

    set(LIBS ${LIBS} jpeg png z)

endif()

add_library(XliImage ${SRC})
target_link_libraries(XliImage ${LIBS})
