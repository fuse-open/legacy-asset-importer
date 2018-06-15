file(GLOB SRC
    ../../include/XliSoundPlayer/*.h
    ../../include/XliSoundPlayer.h)

set(LIBS Xli XliPlatform)

if (XLI_PLATFORM_IOS)

    set(SRC ${SRC} coreaudio/CoreSoundPlayer.mm)

elseif (XLI_PLATFORM_ANDROID)

    set(SRC ${SRC} opensles/SlesSoundPlayer.cpp)
    set(LIBS ${LIBS} OpenSLES)

else()

	set(SRC ${SRC} dummy/DummySoundPlayer.cpp)

endif()

add_library(XliSoundPlayer ${SRC})
target_link_libraries(XliSoundPlayer ${LIBS})
