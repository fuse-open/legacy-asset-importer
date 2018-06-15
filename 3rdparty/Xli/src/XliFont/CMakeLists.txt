set(SRC FontFace.cpp)
set(LIBS Xli XliImage)

set(FREETYPE_INC 
    ../../3rdparty/freetype/include)

set(FREETYPE_SRC
    ../../3rdparty/freetype/src/autofit/autofit.c
    ../../3rdparty/freetype/src/base/ftapi.c
    ../../3rdparty/freetype/src/base/ftbase.c
    ../../3rdparty/freetype/src/base/ftbdf.c
    ../../3rdparty/freetype/src/base/ftbitmap.c
    ../../3rdparty/freetype/src/base/ftdebug.c
    ../../3rdparty/freetype/src/base/ftgasp.c
    ../../3rdparty/freetype/src/base/ftglyph.c
    ../../3rdparty/freetype/src/base/ftgxval.c
    ../../3rdparty/freetype/src/base/ftinit.c
    ../../3rdparty/freetype/src/base/ftlcdfil.c
    ../../3rdparty/freetype/src/base/ftmm.c
    ../../3rdparty/freetype/src/base/ftotval.c
    ../../3rdparty/freetype/src/base/ftpatent.c
    ../../3rdparty/freetype/src/base/ftpfr.c
    ../../3rdparty/freetype/src/base/ftstroke.c
    ../../3rdparty/freetype/src/base/ftsynth.c
    ../../3rdparty/freetype/src/base/ftsystem.c
    ../../3rdparty/freetype/src/base/fttype1.c
    ../../3rdparty/freetype/src/base/ftwinfnt.c
    ../../3rdparty/freetype/src/base/ftxf86.c
    ../../3rdparty/freetype/src/bdf/bdf.c
    ../../3rdparty/freetype/src/cache/ftcache.c
    ../../3rdparty/freetype/src/cff/cff.c
    ../../3rdparty/freetype/src/cid/type1cid.c
    ../../3rdparty/freetype/src/gxvalid/gxvalid.c
    ../../3rdparty/freetype/src/gzip/ftgzip.c
    ../../3rdparty/freetype/src/lzw/ftlzw.c
    ../../3rdparty/freetype/src/otvalid/otvalid.c
    ../../3rdparty/freetype/src/pcf/pcf.c
    ../../3rdparty/freetype/src/pfr/pfr.c
    ../../3rdparty/freetype/src/pfr/pfrgload.c
    ../../3rdparty/freetype/src/pfr/pfrload.c
    ../../3rdparty/freetype/src/pfr/pfrobjs.c
    ../../3rdparty/freetype/src/pfr/pfrsbit.c
    ../../3rdparty/freetype/src/psaux/psaux.c
    ../../3rdparty/freetype/src/pshinter/pshinter.c
    ../../3rdparty/freetype/src/psnames/psnames.c
    ../../3rdparty/freetype/src/raster/raster.c
    ../../3rdparty/freetype/src/sfnt/sfnt.c
    ../../3rdparty/freetype/src/smooth/smooth.c
    ../../3rdparty/freetype/src/truetype/truetype.c
    ../../3rdparty/freetype/src/type1/type1.c
    ../../3rdparty/freetype/src/type42/t42drivr.c
    ../../3rdparty/freetype/src/type42/t42objs.c
    ../../3rdparty/freetype/src/type42/t42parse.c
    ../../3rdparty/freetype/src/winfonts/winfnt.c)

if (XLI_PLATFORM_LINUX)

    include_directories(/usr/include/freetype2)
    set(LIBS ${LIBS} freetype)

else()

    add_definitions(-DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON)
    include_directories(${FREETYPE_INC})
    set(SRC ${SRC} ${FREETYPE_SRC})

endif()

add_library(XliFont ${SRC})
target_link_libraries(XliFont ${LIBS})
