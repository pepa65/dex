#ifndef TERM_H
#define TERM_H

// see termcap(5)
struct term_cap {
	/* boolean caps */
	int ut; // can clear to end of line with bg color set

	/* integer caps */
	int colors;

	/* string caps */
	char *ce; // crear to end of line
	char *ke; // turn keypad off
	char *ks; // turn keypad on
	char *te; // end program that uses cursor motion
	char *ti; // begin program that uses cursor motion
	char *ve; // show cursor
	char *vi; // hide cursor
};

enum term_key_type {
	/* key is character encoded in the current locale's encoding */
	KEY_NORMAL,

	/* same as KEY_NORMAL but with Alt pressed or ESC and KEY_NORMAL */
	KEY_META,

	/* key is one of SKEY_* */
	KEY_SPECIAL,

	/* call term_get_paste() to read pasted text */
	KEY_PASTE,
};

/* special keys */
enum {
	SKEY_BACKSPACE,
	SKEY_INSERT,
	SKEY_DELETE,
	SKEY_HOME,
	SKEY_END,
	SKEY_PAGE_UP,
	SKEY_PAGE_DOWN,
	SKEY_LEFT,
	SKEY_RIGHT,
	SKEY_UP,
	SKEY_DOWN,

	SKEY_F1,
	SKEY_F2,
	SKEY_F3,
	SKEY_F4,
	SKEY_F5,
	SKEY_F6,
	SKEY_F7,
	SKEY_F8,
	SKEY_F9,
	SKEY_F10,
	SKEY_F11,
	SKEY_F12,

	SKEY_SHIFT_LEFT,
	SKEY_SHIFT_RIGHT,

	NR_SKEYS
};

enum {
	COLOR_DEFAULT = -1,
	COLOR_BLACK,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_BLUE,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_GREY
};

enum {
	ATTR_BOLD		= 0x01,
	ATTR_LOW_INTENSITY	= 0x02,
	ATTR_UNDERLINE		= 0x04,
	ATTR_BLINKING		= 0x08,
	ATTR_REVERSE_VIDEO	= 0x10,
	ATTR_INVISIBLE_TEXT	= 0x20,
	ATTR_KEEP		= 0x40,
};


struct term_color {
	short fg;
	short bg;
	unsigned short attr;
};

enum {
	TERM_USE_TERMINFO	= 0x01,
	TERM_USE_TERMCAP	= 0x02,
	TERM_UTF8		= 0x04,
};

extern struct term_cap term_cap;
extern char *term_keycodes[NR_SKEYS];
extern unsigned int term_flags;

/* initialize terminal
 *
 * @term   terminal value or NULL
 * @flags  TERM_* flags
 */
int term_init(const char *term, unsigned int flags);

void term_raw(void);
void term_cooked(void);

int term_read_key(unsigned int *key, enum term_key_type *type);
char *term_read_paste(unsigned int *size);

int term_get_size(int *w, int *h);

const char *term_set_color(const struct term_color *color);

/* move cursor (x and y are zero based) */
const char *term_move_cursor(int x, int y);

/* private - don't use */
int termcap_get_caps(const char *filename, const char *term);
int terminfo_get_caps(const char *filename);

#endif
