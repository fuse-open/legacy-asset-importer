#!/bin/sh

cd "`dirname "$0"`"

copyright-header  --license MIT  \
                  --add-path ../../include/:../../src/ \
                  --guess-extension \
                  --copyright-holder 'Outracks Technologies' \
                  --copyright-software 'Xli' \
                  --copyright-software-description "" \
                  --copyright-year 2010-2014 \
                  --word-wrap 100 \
                  --output-dir .

if [ $? != 0 ]; then
	echo "FAIL: Perhaps you need to do this: sudo gem install copyright-header --user-install"
fi 
