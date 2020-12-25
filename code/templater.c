#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pipe, self-descriptively, pipes the contents from an input file to an output file.
//
// Returns:
//   If the operation succeeds, pipe returns 0.
//   If the operation fails, pipe returns -1 and errno is set.
int pipe(FILE *in, FILE *out) {
  size_t read_size = 16;
  char buf[read_size];

  size_t bytes_read;
  size_t bytes_written;

  while ((bytes_read = fread(buf, 1, read_size, in))) {
    bytes_written = 0;
    size_t new_bytes_written;
    while (bytes_written < bytes_read &&
           (new_bytes_written = fwrite(&buf[bytes_written], 1,
                                       bytes_read - bytes_written, out))) {
      bytes_written += new_bytes_written;
    }

    if (errno != 0) {
      return -1;
    }
  }

  if (errno != 0) {
    return -1;
  }

  return 0;
}

// pipe_from wraps pipe to also open the file in question for reading. See the
// documentation for pipe for information on return values.
int pipe_from(const char *src_file, FILE *out) {
  FILE *in = fopen(src_file, "r");
  if (in == NULL) {
    return -1;
  }
  int ret = pipe(in, out);
  fclose(in);
  return ret;
}

int convert(FILE *in_markdown, FILE *out_html) {
  // TODO: ensure we set errno when we return a non-zero result
  errno = ENOSYS;
  return -1;
}

int main(int argc, char **argv) {
  if (argc < 2 || argc > 5) {
    printf("correct usage:\n");
    printf("  %s <path/to/markdown.md> [path/to/header.html] [path/to/footer.html] [path/to/output.html]\n", argv[0]);
    printf("\n");
    printf("  if the output path is not provided, then this program\n");
    printf("  print the results straight to stdout.\n");
    return 1;
  }

  FILE *in_markdown = fopen(argv[1], "r");
  if (in_markdown == NULL) {
    printf("ERR: failed to open markdown file '%s'.\n", argv[1]);
    printf("     %s\n", strerror(errno));
    return 1;
  }

  FILE *out_html;
  if (argc >= 5) {
    out_html = fopen(argv[4], "w");
    if (out_html == NULL) {
      printf("ERR: failed to open out_html file '%s'.\n", argv[4]);
      printf("     %s\n", strerror(errno));
      return 1;
    }
  } else {
    out_html = stdout;
  }

  if (argc >= 3) {
    const char *header_location = argv[2];
    if (pipe_from(header_location, out_html)) {
      printf("ERR: failed to pipe header '%s'.\n", argv[2]);
      printf("     %s\n", strerror(errno));
      return 1;
    }
  }

  if (convert(in_markdown, out_html)) {
    printf("ERR: failed to map markdown to html.\n");
    printf("     %s\n", strerror(errno));
  }

  if (argc >= 4) {
    const char *footer_location = argv[3];
    if (pipe_from(footer_location, out_html)) {
      printf("ERR: failed to pipe footer '%s'.\n", argv[3]);
      printf("     %s\n", strerror(errno));
    }
  }

  return 0;
}
