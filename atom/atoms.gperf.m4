%{
#include <X11/XCB/xcb.h>
#include <stdlib.h>
#include "xcb_atom.h"

define(`COUNT', 0)dnl
define(`DO', `const xcb_atom_t $1 = { define(`COUNT', incr(COUNT))COUNT };')dnl
include(atomlist.m4)`'dnl
%}

%readonly-tables
%pic
%null-strings
%enum
%includes
%compare-strncmp

%struct-type
struct atom_map { int name; xcb_atom_t value; };
%%
define(`DO', `$1,{ define(`COUNT', incr(COUNT))COUNT }')dnl
include(atomlist.m4)`'dnl
%%
static const char *const atom_names[] = {
define(`DO', `	"$1",')dnl
include(atomlist.m4)`'dnl
};

xcb_atom_t intern_atom_predefined(uint16_t name_len, const char *name)
{
	const struct atom_map *value = in_word_set(name, name_len);
	xcb_atom_t ret = { XCB_NONE };
	if(value)
		ret = value->value;
	return ret;
}

intern_atom_fast_cookie_t intern_atom_fast(xcb_connection_t *c, uint8_t only_if_exists, uint16_t name_len, const char *name)
{
	intern_atom_fast_cookie_t cookie;

	if((cookie.u.atom = intern_atom_predefined(name_len, name)).xid != XCB_NONE)
	{
		cookie.tag = TAG_VALUE;
		return cookie;
	}

	cookie.tag = TAG_COOKIE;
	cookie.u.cookie = xcb_intern_atom(c, only_if_exists, name_len, name);
	return cookie;
}

xcb_atom_t intern_atom_fast_reply(xcb_connection_t *c, intern_atom_fast_cookie_t cookie, xcb_generic_error_t **e)
{
	switch(cookie.tag)
	{
		xcb_intern_atom_reply_t *reply;
	case TAG_VALUE:
		if(e)
			*e = 0;
		break;
	case TAG_COOKIE:
		reply = xcb_intern_atom_reply(c, cookie.u.cookie, e);
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

const char *get_atom_name_predefined(xcb_atom_t atom)
{
	if(atom.xid <= 0 || atom.xid > (sizeof(atom_names) / sizeof(*atom_names)))
		return 0;
	return atom_names[atom.xid - 1];
}

int get_atom_name(xcb_connection_t *c, xcb_atom_t atom, const char **namep, int *lengthp)
{
	static char buf[100];
	const char *name = get_atom_name_predefined(atom);
	int len;
	xcb_get_atom_name_cookie_t atomc;
	xcb_get_atom_name_reply_t *atomr;
	if(name)
	{
		*namep = name;
		*lengthp = strlen(name);
		return 1;
	}
	atomc = xcb_get_atom_name(c, atom);
	atomr = xcb_get_atom_name_reply(c, atomc, 0);
	if(!atomr)
		return 0;
	len = xcb_get_atom_name_name_length(atomr);
	if(len > sizeof(buf))
		len = sizeof(buf);
	*lengthp = len;
	memcpy(buf, xcb_get_atom_name_name(atomr), len);
	*namep = buf;
	free(atomr);
	return 1;
}
