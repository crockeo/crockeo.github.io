#!/usr/bin/env bash

set -e

dir=$(pwd)
if [ ! -f "./code/server" ]; then
    cd code/server
    go build
    cd $dir
fi

echo "serving at 127.0.0.1:3000"
./code/server/server
