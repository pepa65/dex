#ifndef UTIL_H
#define UTIL_H

#include <unistd.h>

unsigned int count_nl(const char *buf, unsigned int size);
unsigned int copy_count_nl(char *dst, const char *src, unsigned int len);
ssize_t xread(int fd, void *buf, size_t count);
ssize_t xwrite(int fd, const void *buf, size_t count);

#endif
