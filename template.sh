#!/usr/bin/env bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
templater="$DIR/code/templater"
if [ ! -f "$templater" ]; then
    original_dir=$(pwd)
    cd "$DIR/code"
    make
    cd "$original_dir"
fi

posts=$(ls -d $DIR/blog/md/*)
for post in $posts; do
    out_name=$(echo $(basename $post) | sed 's/md/html/')
    out_file="$DIR/blog/$out_name"

    $templater "$post" "$DIR/code/header.html" "$DIR/code/footer.html" "$out_file"
done
