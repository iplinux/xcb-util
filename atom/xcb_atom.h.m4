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
} InternAtomFastCookie;

xcb_atom_t InternAtomPredefined(uint16_t name_len, const char *name);
InternAtomFastCookie InternAtomFast(xcb_connection_t *c, uint8_t only_if_exists, uint16_t name_len, const char *name);
xcb_atom_t InternAtomFastReply(xcb_connection_t *c, InternAtomFastCookie cookie, xcb_generic_error_t **e);

const char *GetAtomNamePredefined(xcb_atom_t atom);
int GetAtomName(xcb_connection_t *c, xcb_atom_t atom, const char **namep, int *lengthp);

define(`DO', `extern const xcb_atom_t $1;')dnl
include(atomlist.m4)`'dnl

#endif /* ATOMS_H */
