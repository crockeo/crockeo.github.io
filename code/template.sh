#!/usr/bin/env bash

set -e

md_file="$1"
output_file="$2"
if [ -z "$md_file" ]; then
    echo "ERR: must provide path to markdown file"
    exit 1
fi
if [ -z "$output_file" ]; then
    echo "ERR: must provide output file"
    exit 1
fi

if [ ! -f "$md_file" ]; then
    echo "ERR: markdown file must exist"
    exit 1
fi


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
if [ ! -f "$DIR/templater" ]; then
    original_dir=$(pwd)
    cd "$DIR"
    make
    cd "$original_dir"
fi


cat "$DIR/header.html" > "$output_file"
$DIR/templater "$md_path" >> "$output_file"
cat "$DIR/footer.html" >> "$output_file"
