#!/bin/sh
set -e
cd "`dirname "$0"`"

if [ "$OSTYPE" = "msys" ]; then
    STUFF="Stuff/stuff.exe"
else
    STUFF="mono Stuff/stuff.exe"
fi

$STUFF push Upload --url=https://files.fusetools.com/upload/tooling $@
