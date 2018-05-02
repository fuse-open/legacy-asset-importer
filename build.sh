#!/bin/sh
set -e
cd "`dirname "$0"`"

if [ "$OSTYPE" = "msys" ]; then
    cmd //c build.bat
else
    3rdparty/Xli/scripts/cmakemake "$@" Native

    #set up environment
	MONO_FRAMEWORK_PATH=/Library/Frameworks/Mono.framework/Versions/Current
	export DYLD_FALLBACK_LIBRARY_PATH="$DIR:$MONO_FRAMEWORK_PATH/lib:/lib:/usr/lib"
    xbuild /property:Configuration=Release Uno.Native.sln
fi
