if (XLI_PLATFORM_OSX)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lobjc -Wl,-framework,Cocoa")
endif()

file(GLOB SRC *.cpp *.h)
add_library(uAssetPlus SHARED ${SRC})
target_link_libraries(uAssetPlus uAsset Fonts XliImage XliZip)
