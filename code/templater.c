#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fputc except that it HTML-code escapes characters if applicable
int fputc_esc(char c, FILE *out) {
  const char *output_str;
  if (c == '<') {
    output_str = "&lt;";
  } else if (c == '>') {
    output_str = "&gt;";
  } else {
    return fputc(c, out);
  }

  return fprintf(out, "%s", output_str);
}

// consumes input from a file so long as the character consumed is one of the
// characters in the parameter `options`. also contains an explicit length
// parameter because depending on null-terminated strings is a one-way trip to
// memory unsafety-ville.
int take_oneof(FILE *in, const char *options, size_t len) {
  int count = 0;
  char c;
  while ((c = fgetc(in)) != EOF) {
    bool found_option = false;
    for (size_t i = 0; i < len; i++) {
      if (c == options[i]) {
        found_option = true;
        count += 1;
        break;
      }
    }

    if (!found_option) {
      ungetc(c, in);
      break;
    }
  }

  if (errno != 0) {
    return -1;
  }

  return count;
}

// reads characters in from the provided file until any one of the provided
// characters is encountered.
char *take_until(FILE *in, const char *options, size_t len) {
  size_t size = 512;
  size_t pos = 0;

  char *buf = (char *)malloc(sizeof(char) * size);
  char c;
  while ((c = fgetc(in)) != EOF) {
    bool found_option = false;
    for (size_t i = 0; i < len; i++) {
      if (c == options[i]) {
        found_option = true;
        break;
      }
    }

    if (found_option) {
      ungetc(c, in);
      break;
    }

    buf[pos] = c;
    pos++;
    // resize at (size - 1) so we have space for the null terminator
    if (pos >= size - 1) {
      char *new_buf = (char *)malloc(sizeof(char) * 2 * size);
      memcpy(new_buf, buf, sizeof(char) * pos);
      free(buf);
      buf = new_buf;
    }
  }

  if (c == EOF && errno != 0) {
    free(buf);
    return NULL;
  }

  buf[pos] = '\0';
  return buf;
}

// peeks at the next value in the provided file.
int peek(FILE *in) {
  int result = fgetc(in);
  if (result != EOF) {
    ungetc(result, in);
  }
  return result;
}

// pipe, self-descriptively, pipes the contents from an input file to an output
// file.
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
      printf("ERR: (pipe) failed to write\n");
      return -1;
    }
  }

  if (errno != 0) {
    printf("ERR: (pipe) failed to read\n");
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

int convert_link(FILE *in_markdown, FILE *out_html) {
  if (fgetc(in_markdown) != '[') {
    printf("ERR: (convert_link) link text must start with '['\n");
    errno = EINVAL;
    return -1;
  }

  char *text = take_until(in_markdown, "]", 1);
  if (text == NULL) {
    printf("ERR: (convert_link) failed to read link text\n");
    return -1;
  }

  if (fgetc(in_markdown) != ']') {
    printf("ERR: (convert_link) link text must end with ']'\n");
    errno = EINVAL;
    free(text);
    return -1;
  }

  if (fgetc(in_markdown) != '(') {
    printf("ERR: (convert_link) link URL must start with '('\n");
    errno = EINVAL;
    free(text);
    return -1;
  }

  char *url = take_until(in_markdown, ")", 1);
  if (url == NULL) {
    printf("ERR: (convert_link) failed to read link URL\n");
    free(text);
    return -1;
  }

  if (fgetc(in_markdown) != ')') {
    printf("ERR: (convert_link) link URL must end with ')'\n");
    errno = EINVAL;
    free(text);
    free(url);
    return -1;
  }

  fprintf(out_html, "<a href=\"%s\">%s</a>", url, text);

  free(text);
  free(url);

  return 0;
}

int convert_text(FILE *in_markdown, FILE *out_html) {
  bool in_italic = false;
  bool in_bold = false;
  bool in_code = false;

  fprintf(out_html, "<p>");
  char c;
  while ((c = fgetc(in_markdown)) != EOF) {
    if (c == '\r' || c == '\n') {
      int newlines = take_oneof(in_markdown, "\r\n", 2);
      if (newlines >= 1) {
        break;
      }
      fputc_esc(' ', out_html);
    } else if (c == '_') {
      if (!in_italic) {
        fprintf(out_html, "<i>");
        in_italic = true;
      } else {
        fprintf(out_html, "</i>");
        in_italic = false;
      }
    } else if (c == '*') {
      if (!in_bold) {
        fprintf(out_html, "<b>");
        in_bold = true;
      } else {
        fprintf(out_html, "</b>");
        in_bold = false;
      }
    } else if (c == '`') {
      if (!in_code) {
        fprintf(out_html, "<code>");
        in_code = true;
      } else {
        fprintf(out_html, "</code>");
        in_code = false;
      }
    } else if (c == '[') {
      ungetc(c, in_markdown);
      if (convert_link(in_markdown, out_html)) {
        return -1;
      }
    } else {
      fputc_esc(c, out_html);
    }
  }
  fprintf(out_html, "</p>\n");
  return 0;
}

int convert_header(FILE *in_markdown, FILE *out_html) {
  int hash_count = take_oneof(in_markdown, "#", 1);
  if (hash_count < 0) {
    return -1;
  }

  if (hash_count >= 6) {
    for (int i = 0; i < hash_count; i++) {
      if (fputc_esc('#', out_html) == EOF) {
        return -1;
      }
    }
  }

  take_oneof(in_markdown, " \t", 2);

  fprintf(out_html, "<h%d class=\"highlight-purple\">", hash_count);
  convert_text(in_markdown, out_html);
  fprintf(out_html, "</h%d>\n", hash_count);

  return 0;
}

int convert_ordered_list(FILE *in_markdown, FILE *out_html) { return 0; }

int convert_unordered_list(FILE *in_markdown, FILE *out_html) { return 0; }

int convert_code_block(FILE *in_markdown, FILE *out_html) {
  int ticks = take_oneof(in_markdown, "`", 1);
  if (ticks < 3) {
    // if we don't get the 3 ticks, we just treat this block like text instead
    ungetc('`', in_markdown);
    return convert_text(in_markdown, out_html);
  }

  fprintf(out_html, "<pre><code>");

  char c;
  while ((c = fgetc(in_markdown)) != EOF && c != '`') {
    fputc_esc(c, out_html);
  }
  if (c == EOF && errno != 0) {
    // TODO: print error
    return -1;
  }
  take_oneof(in_markdown, "`", 1);

  fprintf(out_html, "</code></pre>");

  return 0;
}

int convert_image(FILE *in_markdown, FILE *out_html) {
  if (fgetc(in_markdown) != '!') {
    printf("ERR: (convert_image) image must start '!'\n");
    errno = EINVAL;
    return -1;
  }

  if (fgetc(in_markdown) != '[') {
    printf("ERR: (convert_image) image alt must start with '['\n");
    errno = EINVAL;
    return -1;
  }

  char *alt = take_until(in_markdown, "]", 1);
  if (alt == NULL) {
    printf("ERR: (convert_image) failed to read image alt\n");
    return -1;
  }

  if (fgetc(in_markdown) != ']') {
    printf("ERR: (convert_image) image alt must end with ']'\n");
    errno = EINVAL;
    free(alt);
    return -1;
  }

  if (fgetc(in_markdown) != '(') {
    printf("ERR: (convert_image) image URL must start with '('\n");
    errno = EINVAL;
    free(alt);
    return -1;
  }

  char *url = take_until(in_markdown, ")", 1);
  if (url == NULL) {
    printf("ERR: (convert_image) failed to read image URL\n");
    free(alt);
    return -1;
  }

  if (fgetc(in_markdown) != ')') {
    printf("ERR: (convert_image) image URL must end with ')'\n");
    errno = EINVAL;
    free(alt);
    free(url);
    return -1;
  }

  fprintf(out_html, "<img src=\"%s\" alt=\"%s\" />", url, alt);

  free(alt);
  free(url);

  return 0;
}

int convert(FILE *in_markdown, FILE *out_html) {
  char c;
  while ((c = peek(in_markdown)) != EOF) {
    if (isspace(c)) {
      // consume whitespace between lines
      fgetc(in_markdown);
    } else if (c == '#') {
      if (convert_header(in_markdown, out_html)) {
        return -1;
      }
    } else if (c == '[') {
      if (convert_link(in_markdown, out_html)) {
        return -1;
      }
    } else if (c == '`') {
      // TODO: this isn't a great signal; we'd prefer to peek at 3 consecutive
      // characters
      if (convert_code_block(in_markdown, out_html)) {
        return -1;
      }
    } else if (c == '!') {
      if (convert_image(in_markdown, out_html)) {
        return -1;
      }
    } else {
      if (convert_text(in_markdown, out_html)) {
        return -1;
      }
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2 || argc > 5) {
    printf("correct usage:\n");
    printf("  %s <path/to/markdown.md> [path/to/header.html] "
           "[path/to/footer.html] [path/to/output.html]\n",
           argv[0]);
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
    return 1;
  }

  if (argc >= 4) {
    const char *footer_location = argv[3];
    if (pipe_from(footer_location, out_html)) {
      printf("ERR: failed to pipe footer '%s'.\n", argv[3]);
      printf("     %s\n", strerror(errno));
      return 1;
    }
  }

  return 0;
}
