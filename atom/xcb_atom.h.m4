#ifndef ATOMS_H
#define ATOMS_H

enum tag_t {
	TAG_COOKIE,
	TAG_VALUE
};
typedef struct {
	enum tag_t tag;
	union {
		xcb_intern_atom_cookie_t cookie;
		xcb_atom_t atom;
	} u;
} intern_atom_fast_cookie_t;

xcb_atom_t intern_atom_predefined(uint16_t name_len, const char *name);
intern_atom_fast_cookie_t intern_atom_fast(xcb_connection_t *c, uint8_t only_if_exists, uint16_t name_len, const char *name);
xcb_atom_t intern_atom_fast_reply(xcb_connection_t *c, intern_atom_fast_cookie_t cookie, xcb_generic_error_t **e);

const char *get_atom_name_predefined(xcb_atom_t atom);
int get_atom_name(xcb_connection_t *c, xcb_atom_t atom, const char **namep, int *lengthp);

define(`DO', `extern const xcb_atom_t $1;')dnl
include(atomlist.m4)`'dnl

#endif /* ATOMS_H */
