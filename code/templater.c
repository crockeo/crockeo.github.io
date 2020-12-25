#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int convert(FILE *in_markdown, FILE *out_html) {
    // TODO: ensure we set errno when we return a non-zero result
    errno = ENOSYS;
    return -1;
}

// read in markdown file
//
// convert to html file

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        printf("correct usage:\n");
        printf("  %s <path/to/markdown.md> [path/to/output.html]\n", argv[0]);
        printf("\n");
        printf("  if the output path is not provided, then this program\n");
        printf("  print the results straight to stdout.\n");
        return 1;
    }

    FILE *in_markdown = fopen(argv[1], "r");
    if (in_markdown == NULL) {
        printf("ERR: failed to open markdown file '%s'.\n", argv[1]);
        printf("     %s\n", strerror(errno));;
        return 1;
    }

    FILE *out_html;
    if (argc == 3) {
        out_html = fopen(argv[2], "w");
        if (out_html == NULL) {
            printf("ERR: failed to open out_html file '%s'.\n", argv[2]);
            printf("     %s\n", strerror(errno));
            return 1;
        }
    } else {
        out_html = stdout;
    }

    if (convert(in_markdown, out_html)) {
        printf("ERR: failed to map markdown to html.\n");
        printf("     %s\n", strerror(errno));
    }

    return 0;
}
