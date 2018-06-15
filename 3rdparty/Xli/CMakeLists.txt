project("Xli")

cmake_minimum_required(VERSION 2.8)

include(cmake/modules/XliLibSetup.cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${LIBRARY_OUTPUT_PATH})

if (APPLE)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality -Wno-extra-tokens -Wno-deprecated-declarations")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum -Wno-logical-op-parentheses -Wno-parentheses-equality")

elseif (XLI_PLATFORM_LINUX)
    
    # Fixes linker problem on Ubuntu
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-stack-protector")

endif()

add_subdirectory(src/Xli)

if (NOT XLI_NO_IMAGE)
    add_subdirectory(src/XliImage)
endif()

if (NOT XLI_NO_FONT)
    add_subdirectory(src/XliFont)
endif()

if (NOT XLI_NO_JSON)
    add_subdirectory(src/XliJson)
endif()

if (NOT XLI_NO_ZIP)
    add_subdirectory(src/XliZip)
endif()
