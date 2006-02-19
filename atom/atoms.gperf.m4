%{
#include <X11/XCB/xcb.h>
#include <stdlib.h>
#include "xcb_atom.h"

define(`COUNT', 0)dnl
define(`DO', `const XCBATOM $1 = { define(`COUNT', incr(COUNT))COUNT };')dnl
include(atomlist.m4)`'dnl
%}

%readonly-tables
%pic
%null-strings
%enum
%includes
%compare-strncmp

%struct-type
struct atom_map { int name; XCBATOM value; };
%%
define(`DO', `$1,{ define(`COUNT', incr(COUNT))COUNT }')dnl
include(atomlist.m4)`'dnl
%%
static const char *const atom_names[] = {
define(`DO', `	"$1",')dnl
include(atomlist.m4)`'dnl
};

XCBATOM InternAtomPredefined(CARD16 name_len, const char *name)
{
	const struct atom_map *value = in_word_set(name, name_len);
	XCBATOM ret = { None };
	if(value)
		ret = value->value;
	return ret;
}

InternAtomFastCookie InternAtomFast(XCBConnection *c, BOOL only_if_exists, CARD16 name_len, const char *name)
{
	InternAtomFastCookie cookie;

	if((cookie.u.atom = InternAtomPredefined(name_len, name)).xid != None)
	{
		cookie.tag = TAG_VALUE;
		return cookie;
	}

	cookie.tag = TAG_COOKIE;
	cookie.u.cookie = XCBInternAtom(c, only_if_exists, name_len, name);
	return cookie;
}

XCBATOM InternAtomFastReply(XCBConnection *c, InternAtomFastCookie cookie, XCBGenericError **e)
{
	switch(cookie.tag)
	{
		XCBInternAtomRep *reply;
	case TAG_VALUE:
		if(e)
			*e = 0;
		break;
	case TAG_COOKIE:
		reply = XCBInternAtomReply(c, cookie.u.cookie, e);
		if(reply)
		{
			cookie.u.atom = reply->atom;
			free(reply);
		}
		else
			cookie.u.atom.xid = 0;
		break;
	}
	return cookie.u.atom;
}

const char *GetAtomNamePredefined(XCBATOM atom)
{
	if(atom.xid <= 0 || atom.xid > (sizeof(atom_names) / sizeof(*atom_names)))
		return 0;
	return atom_names[atom.xid - 1];
}

int GetAtomName(XCBConnection *c, XCBATOM atom, const char **namep, int *lengthp)
{
	static char buf[100];
	const char *name = GetAtomNamePredefined(atom);
	int len;
	XCBGetAtomNameCookie atomc;
	XCBGetAtomNameRep *atomr;
	if(name)
	{
		*namep = name;
		*lengthp = strlen(name);
		return 1;
	}
	atomc = XCBGetAtomName(c, atom);
	atomr = XCBGetAtomNameReply(c, atomc, 0);
	if(!atomr)
		return 0;
	len = XCBGetAtomNameNameLength(atomr);
	if(len > sizeof(buf))
		len = sizeof(buf);
	*lengthp = len;
	memcpy(buf, XCBGetAtomNameName(atomr), len);
	*namep = buf;
	free(atomr);
	return 1;
}
