file(GLOB SRC 
    ../../include/XliGL/*.h
    ../../include/XliGL.h
    *.cpp)

set(LIBS Xli)

if (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC})
    set(LIBS ${LIBS} android log EGL GLESv2)

elseif (XLI_PLATFORM_IOS)

    set(SRC ${SRC})

elseif (XLI_PLATFORM_OSX)

    add_definitions(-DXLI_HAS_GLEW -DGLEW_STATIC)
    include_directories(../../3rdparty/SDL2/include)
    include_directories(../../3rdparty/glew/include)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")
    set(SRC ${SRC} ../../3rdparty/glew/src/glew.c sdl2/SDL2GLContext.cpp)

elseif (XLI_PLATFORM_LINUX)

    add_definitions(`sdl2-config --cflags`)
    set(SRC ${SRC} sdl2/SDL2GLContext.cpp)
    set(LIBS ${LIBS} XliPlatform SDL2)

    if (NOT XLI_ARCH_ARM)
        add_definitions(-DXLI_HAS_GLEW)
        set(LIBS ${LIBS} GL GLEW)
    endif()

elseif (XLI_PLATFORM_WIN32)

    add_definitions(-DXLI_HAS_GLEW -DGLEW_STATIC)
    include_directories(../../3rdparty/glew/include)
    set(SRC ${SRC} wgl/WGLContext.cpp ../../3rdparty/glew/src/glew.c)

    if (MSVC)
        set(CMAKE_STATIC_LINKER_FLAGS opengl32.lib)
    endif()

endif()

add_library(XliGL ${SRC})
target_link_libraries(XliGL ${LIBS})
