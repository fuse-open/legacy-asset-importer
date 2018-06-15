#!/bin/sh
set -e
cd "`dirname "$0"`"
scripts/cmakemake "$@" --platform=iOS .
