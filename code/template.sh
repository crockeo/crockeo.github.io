#!/usr/bin/env bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
if [ ! -f "$DIR/templater" ]; then
    original_dir=$(pwd)
    cd "$DIR"
    make
    cd "$original_dir"
fi
$DIR/templater $@
