#ifndef ATOMS_H
#define ATOMS_H

enum tag_t {
	TAG_COOKIE,
	TAG_VALUE
};
typedef struct {
	enum tag_t tag;
	union {
		XCBInternAtomCookie cookie;
		XCBATOM atom;
	} u;
} InternAtomFastCookie;

XCBATOM InternAtomPredefined(CARD16 name_len, const char *name);
InternAtomFastCookie InternAtomFast(XCBConnection *c, BOOL only_if_exists, CARD16 name_len, const char *name);
XCBATOM InternAtomFastReply(XCBConnection *c, InternAtomFastCookie cookie, XCBGenericError **e);

const char *GetAtomNamePredefined(XCBATOM atom);
int GetAtomName(XCBConnection *c, XCBATOM atom, const char **namep, int *lengthp);

define(`DO', `extern const XCBATOM $1;')dnl
include(atomlist.m4)`'dnl

#endif /* ATOMS_H */
