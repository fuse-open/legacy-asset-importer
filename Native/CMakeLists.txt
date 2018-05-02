project("uAsset")
cmake_minimum_required(VERSION 2.8.9)

# Variables
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/../3rdparty)
set(XLI_DIR ${THIRDPARTY_DIR}/Xli)

# Include Xli
add_subdirectory(${XLI_DIR} ${CMAKE_CURRENT_BINARY_DIR}/Xli)
include(${XLI_DIR}/cmake/modules/XliLibSetup.cmake)
include(${XLI_DIR}/Xli.cmake)

if (XLI_PLATFORM_OSX)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
endif()

# Configuration
include_directories(include src)

# Build static libraries
add_subdirectory(src/Fonts)

# Build shared libraries
add_subdirectory(src/uAsset)
add_subdirectory(src/uAssetPlus)
