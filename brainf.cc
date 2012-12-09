
#include <stdio.h>
#include "parse.h"
#include "reader.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <input-file>\n", argv[0]);
    return 1;
  }

  string err;
  auto contents = ReadFile(argv[1], &err);
  if (!err.empty()) {
    fprintf(stderr, "error: %s\n", err.c_str());
    return 1;
  }

  auto ast = Parse(contents);
  puts(ast->ToString().c_str());
  return 0;
}
