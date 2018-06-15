#!/bin/sh
set -ex
cd "`dirname "$0"`"

SHA=`git rev-parse HEAD`
DATE=`date +"%y%m%d"`
VERSION="$DATE-${SHA:0:6}"
BASENAME="Xli-$VERSION-OSX"

rm -rf lib
sh build.sh
zip -r "$BASENAME.zip" include lib
