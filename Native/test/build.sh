#!/bin/sh
set -e
cd "`dirname "$0"`"
#../build.sh "$@"
../../3rdparty/Xli/scripts/cmakemake "$@" .
