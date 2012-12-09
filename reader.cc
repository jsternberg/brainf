
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "reader.h"

string ReadFile(const string& path, string *err) {
  FILE *fd = fopen(path.c_str(), "r");
  if (!fd) {
    err->assign(strerror(errno));
    return string();
  }

  const size_t buf_size = 4096;
  string contents;
  char buf[buf_size];
  size_t bytes_read;
  while ((bytes_read = fread(buf, sizeof(char), buf_size, fd)) > 0) {
    contents.append(buf, bytes_read);
  }

  if (ferror(fd)) {
    err->assign(strerror(errno));
    fclose(fd);
    return string();
  }

  fclose(fd);
  return contents;
}
