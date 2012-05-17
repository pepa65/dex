#include "config.h"
#include "command.h"
#include "error.h"
#include "gbuf.h"
#include "common.h"

const char *config_file;
int config_line;

const char *reset_colors_rc =
"hi default\n"
"hi nontext blue keep\n"
"hi noline blue\n"
"hi wserror default yellow\n"
"hi selection keep gray keep\n"
"hi currentline keep keep keep\n"
"hi linenumber\n"
"hi statusline black gray\n"
"hi commandline\n"
"hi errormsg bold red\n"
"hi infomsg bold blue\n"
"hi tabbar black gray\n"
"hi activetab bold\n"
"hi inactivetab black gray\n";

static int is_command(const char *str, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		if (str[i] == '#')
			return 0;
		if (!isspace(str[i]))
			return 1;
	}
	return 0;
}

// odd number of backslashes at end of line?
static int has_line_continuation(const char *str, int len)
{
	int pos = len - 1;

	while (pos >= 0 && str[pos] == '\\')
		pos--;
	return (len - 1 - pos) % 2;
}

void exec_config(const struct command *cmds, const char *buf, size_t size)
{
	const char *ptr = buf;
	GBUF(line);

	while (ptr < buf + size) {
		size_t n = buf + size - ptr;
		char *end = memchr(ptr, '\n', n);

		if (end)
			n = end - ptr;

		if (line.len || is_command(ptr, n)) {
			if (has_line_continuation(ptr, n)) {
				gbuf_add_buf(&line, ptr, n - 1);
			} else {
				gbuf_add_buf(&line, ptr, n);
				handle_command(cmds, line.buffer);
				gbuf_clear(&line);
			}
		}
		config_line++;
		ptr += n + 1;
	}
	if (line.len)
		handle_command(cmds, line.buffer);
	gbuf_free(&line);
}

int do_read_config(const struct command *cmds, const char *filename, int must_exist)
{
	char *buf;
	ssize_t size = read_file(filename, &buf);

	if (size < 0) {
		if (errno != ENOENT || must_exist)
			error_msg("Error reading %s: %s", filename, strerror(errno));
		return -1;
	}

	config_file = filename;
	config_line = 1;

	exec_config(cmds, buf, size);
	free(buf);
	return 0;
}

int read_config(const struct command *cmds, const char *filename, int must_exist)
{
	/* recursive */
	const char *saved_config_file = config_file;
	int saved_config_line = config_line;
	int ret = do_read_config(cmds, filename, must_exist);
	config_file = saved_config_file;
	config_line = saved_config_line;
	return ret;
}

void exec_builtin_rc(const char *rc)
{
	// no need to change filename because there can't be any errors
	int saved_config_line = config_line;
	exec_config(commands, rc, strlen(rc));
	config_line = saved_config_line;
}
