#!/usr/bin/env bash

set -e

# ensuring we have access to the templater
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
templater="$DIR/code/templater"
if [ ! -f "$templater" ]; then
    original_dir=$(pwd)
    cd "$DIR/code"
    make
    cd "$original_dir"
fi

# templating each of the blogs
posts=$(ls -d $DIR/blog/md/*)
for post in $posts; do
    out_name=$(echo $(basename $post) | sed 's/md/html/')
    out_file="$DIR/blog/$out_name"

    $templater "$post" "$DIR/code/header.html" "$DIR/code/footer.html" "$out_file"
done

cat "$DIR/code/header.html" > blog.html
cat << EndOfFile >> blog.html
<h1>blog posts</h1>
EndOfFile
for post in $posts; do
    out_name=$(echo $(basename $post) | sed 's/md/html/')

    day=$(echo $out_name | sed -E 's/([0-9]+-[0-9]+-[0-9]+)-.+/\1/')
    name=$(echo $out_name | sed -E 's/[0-9]+-[0-9]+-[0-9]+-//' | sed -E 's/\.html//' | sed -E 's/-/ /g')
    cat << EndOfFile >> blog.html
<h3>$day <a href="/blog/$out_name">$name</a></h3>
EndOfFile
done
cat "$DIR/code/footer.html" >> blog.html
