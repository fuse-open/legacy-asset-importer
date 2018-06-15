file(GLOB SRC
    ../../include/XliAudio/*.h
    ../../include/XliAudio.h
    portaudio/*.cpp
    *.cpp)

if (WIN32)
    
    include_directories(../../3rdparty/portaudio/include)

elseif (APPLE)

    include_directories(/opt/local/include)
    link_directories(/opt/local/lib)

endif()

add_library(XliAudio ${SRC})
target_link_libraries(XliAudio Xli portaudio)
