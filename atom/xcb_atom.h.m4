#ifndef ATOMS_H
#define ATOMS_H

enum xcb_atom_fast_tag_t {
	TAG_COOKIE,
	TAG_VALUE
};
typedef struct {
	enum xcb_atom_fast_tag_t tag;
	union {
		xcb_intern_atom_cookie_t cookie;
		xcb_atom_t atom;
	} u;
} xcb_atom_fast_cookie_t;

xcb_atom_t xcb_atom_get_predefined(uint16_t name_len, const char *name);
xcb_atom_fast_cookie_t xcb_atom_get_fast(xcb_connection_t *c, uint8_t only_if_exists, uint16_t name_len, const char *name);
xcb_atom_t xcb_atom_get_fast_reply(xcb_connection_t *c, xcb_atom_fast_cookie_t cookie, xcb_generic_error_t **e);

const char *xcb_atom_get_name_predefined(xcb_atom_t atom);
int xcb_atom_get_name(xcb_connection_t *c, xcb_atom_t atom, const char **namep, int *lengthp);

char *xcb_atom_name_by_screen(const char *base, uint8_t screen);
char *xcb_atom_name_by_resource(const char *base, uint32_t resource);
char *xcb_atom_name_unique(const char *base, uint32_t id);

define(`DO', `extern const xcb_atom_t $1;')dnl
include(atomlist.m4)`'dnl

#endif /* ATOMS_H */
