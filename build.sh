#!/usr/bin/env bash

set -e
cd `dirname $0`

autoreconf -fi
if [ -z "$PREFIX" ];then
    ./configure
else
    if [ "/" -eq "${PREFIX:0:1}" ]; then
        ./configure --prefix="$PREFIX"
    else
        ./configure --prefix="$OLDPWD/$PREFIX"
    fi
fi

make
make install
