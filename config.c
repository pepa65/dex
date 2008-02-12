#include "buffer.h"

#include <sys/mman.h>

void read_config(void)
{
	const char *home = getenv("HOME");
	char filename[1024];
	struct stat st;
	size_t size, alloc = 0;
	char *buf, *ptr, *line = NULL;
	int fd;

	if (!home)
		home = "";

	snprintf(filename, sizeof(filename), "%s/.editor/rc", home);
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		return;
	}
	fstat(fd, &st);
	size = st.st_size;
	if (!size) {
		close(fd);
		return;
	}
	buf = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);
	if (buf == MAP_FAILED) {
		return;
	}

	ptr = buf;
	while (ptr < buf + size) {
		size_t n = buf + size - ptr;
		char *end = memchr(ptr, '\n', n);

		if (end)
			n = end - ptr;

		if (alloc < n + 1) {
			alloc = (n + 1 + 63) & ~63;
			xrenew(line, alloc);
		}
		memcpy(line, ptr, n);
		line[n] = 0;
		handle_command(line);

		ptr += n + 1;
	}
	munmap(buf, st.st_size);
}
