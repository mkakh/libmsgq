#!/usr/bin/env bash

set -eu
autoreconf -fi
if [ -z "$PREFIX" ];then
    make
    make install
else
    make --prefix="$PREFIX"
    make install
