#!/bin/bash
set -e
cd "`dirname "$0"`"

# TeamCity sets $BUILD_NUMBER, so use that if set
if [ -n "$BUILD_NUMBER" ]; then
    VERSION="$BUILD_NUMBER"
else
    SHA=`git rev-parse HEAD`
    DATE=`date +"%y%m%d"`
    VERSION="$DATE-${SHA:0:6}"
fi

rm -rf lib Release Upload
mkdir -p Release

./build.sh
cp Source/Uno.Native/bin/Release/* Release

case $OSTYPE in
darwin*)
    ROOT=`pwd -P`
    LIBS="$ROOT/Native/lib/libuAsset.dylib
$ROOT/Native/lib/libuAssetPlus.dylib"

    IFS=$'\n'
    for lib in $LIBS; do
        name=`basename "$lib"`
        dst="$ROOT/Native/lib/$name"

        if [ "$lib" != "$dst" ]; then
            cp -fv "$lib" "$dst"
        fi

        for dlib in $LIBS; do
            install_name_tool -change "$dlib" "@loader_path/`basename "$dlib"`" "$dst"
            install_name_tool -change "@executable_path/`basename "$dlib"`" "@loader_path/`basename "$dlib"`" "$dst"
            install_name_tool -change "`basename "$dlib"`" "@loader_path/`basename "$dlib"`" "$dst"
        done

        install_name_tool -id "$name" "$dst"
        strip -x "$dst"
        otool -L "$dst"
    done

    cp Native/lib/*.dylib Release

    mono Stuff/stuff.exe pack Release \
       --name=Uno.Native \
        --suffix=-$VERSION-macOS \
        --output-dir=Upload
    ;;

linux*)
    if [ "`getconf LONG_BIT`" = "64" ]; then
        ARCH="x86_64"
        ARCH_SHORT="x64"
    else
        ARCH="x86_32"
        ARCH_SHORT="x86"
    fi

    cp -fuv Native/lib/*.so \
        3rdparty/PVRTexTool/Linux_$ARCH/Dynamic/libPVRTexLib.so \
        3rdparty/Xli/lib/libXli.so \
        3rdparty/Xli/lib/libXliFont.so \
        3rdparty/Xli/lib/libXliImage.so \
        3rdparty/Xli/lib/libXliZip.so \
        Release

    mono Stuff/stuff.exe pack Release \
       --name=Uno.Native \
        --suffix=-$VERSION-Linux-$ARCH \
        --output-dir=Upload
    ;;

msys*)
    mkdir -p Release/x86 Release/x64
    cp Native/lib/vs2013/x86/Release/*.dll Release/x86
    cp Native/lib/vs2013/x64/Release/*.dll Release/x64

    Stuff/stuff.exe pack Release \
        --name=Uno.Native \
        --suffix=-$VERSION-Win32 \
        --output-dir=Upload
    ;;
esac
