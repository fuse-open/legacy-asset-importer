#!/bin/sh
set -ex
cd "`dirname "$0"`"

SHA=`git rev-parse HEAD`
DATE=`date +"%y%m%d"`
VERSION="$DATE-${SHA:0:6}"
BASENAME="Xli-$VERSION-Win32"

rm -rf lib
cmd //c build-vs2013.bat
"C:\Program Files\7-Zip\7z.exe" a -y "$BASENAME.zip" include lib
