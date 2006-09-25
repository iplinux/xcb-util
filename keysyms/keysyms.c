#include <stdlib.h>

#include <xcb/xcb.h>
#define XK_MISCELLANY
#define XK_XKB_KEYS
#define XK_LATIN1
#define XK_LATIN2
#define XK_LATIN3
#define XK_LATIN4
#define XK_CYRILLIC
#define XK_GREEK
#define XK_ARMENIAN
#include <X11/keysymdef.h>

#include "xcb_keysyms.h"

/* Private declaration */
enum tag_t {
  TAG_COOKIE,
  TAG_VALUE
};

struct _XCBKeySymbols
{
  xcb_connection_t *c;
  enum tag_t     tag;
  union {
    xcb_get_keyboard_mapping_cookie_t cookie;
    xcb_get_keyboard_mapping_reply_t *reply;
  } u;
};

static void xcb_convert_case(xcb_keysym_t  sym,
			   xcb_keysym_t *lower,
			   xcb_keysym_t *upper);

static void xcb_key_symbols_get_reply (xcb_key_symbols_t    *syms,
				   xcb_generic_error_t **e);

/* public implementation */

xcb_key_symbols_t *
xcb_key_symbols_alloc (xcb_connection_t *c)
{
  xcb_key_symbols_t *syms;
  xcb_keycode_t     min_keycode;
  xcb_keycode_t     max_keycode;

  if (!c)
    return NULL;

  syms = (xcb_key_symbols_t *)malloc (sizeof (xcb_key_symbols_t));

  syms->c = c;
  syms->tag = TAG_COOKIE;

  min_keycode = xcb_get_setup (c)->min_keycode;
  max_keycode = xcb_get_setup (c)->max_keycode;
  
  syms->u.cookie = xcb_get_keyboard_mapping(c,
					 min_keycode,
					 max_keycode.id - min_keycode.id + 1);
  
  return syms;
}

void
xcb_key_symbols_free (xcb_key_symbols_t *syms)
{
  if (syms)
    {
      if (syms->tag == TAG_VALUE)
	free (syms->u.reply);
      free (syms);
      syms = NULL;
    }
}

/*  Use of the 'col' parameter:

A list of KeySyms is associated with each KeyCode. The list is intended
to convey the set of symbols on the corresponding key. If the list
(ignoring trailing NoSymbol entries) is a single KeySym ``K'', then the
list is treated as if it were the list ``K NoSymbol K NoSymbol''. If the
list (ignoring trailing NoSymbol entries) is a pair of KeySyms ``K1
K2'', then the list is treated as if it were the list ``K1 K2 K1 K2''.
If the list (ignoring trailing NoSymbol entries) is a triple of KeySyms
``K1 K2 K3'', then the list is treated as if it were the list ``K1 K2 K3
NoSymbol''. When an explicit ``void'' element is desired in the list,
the value VoidSymbol can be used.

The first four elements of the list are split into two groups of
KeySyms. Group 1 contains the first and second KeySyms; Group 2 contains
the third and fourth KeySyms. Within each group, if the second element
of the group is NoSymbol , then the group should be treated as if the
second element were the same as the first element, except when the first
element is an alphabetic KeySym ``K'' for which both lowercase and
uppercase forms are defined. In that case, the group should be treated
as if the first element were the lowercase form of ``K'' and the second
element were the uppercase form of ``K.''

The standard rules for obtaining a KeySym from a KeyPress event make use
of only the Group 1 and Group 2 KeySyms; no interpretation of other
KeySyms in the list is given. Which group to use is determined by the
modifier state. Switching between groups is controlled by the KeySym
named MODE SWITCH, by attaching that KeySym to some KeyCode and
attaching that KeyCode to any one of the modifiers Mod1 through Mod5.
This modifier is called the group modifier. For any KeyCode, Group 1 is
used when the group modifier is off, and Group 2 is used when the group
modifier is on.

The Lock modifier is interpreted as CapsLock when the KeySym named
XK_Caps_Lock is attached to some KeyCode and that KeyCode is attached to
the Lock modifier. The Lock modifier is interpreted as ShiftLock when
the KeySym named XK_Shift_Lock is attached to some KeyCode and that
KeyCode is attached to the Lock modifier. If the Lock modifier could be
interpreted as both CapsLock and ShiftLock, the CapsLock interpretation
is used.

The operation of keypad keys is controlled by the KeySym named
XK_Num_Lock, by attaching that KeySym to some KeyCode and attaching that
KeyCode to any one of the modifiers Mod1 through Mod5 . This modifier is
called the numlock modifier. The standard KeySyms with the prefix
``XK_KP_'' in their name are called keypad KeySyms; these are KeySyms
with numeric value in the hexadecimal range 0xFF80 to 0xFFBD inclusive.
In addition, vendor-specific KeySyms in the hexadecimal range 0x11000000
to 0x1100FFFF are also keypad KeySyms.

Within a group, the choice of KeySym is determined by applying the first
rule that is satisfied from the following list:

* The numlock modifier is on and the second KeySym is a keypad KeySym. In
  this case, if the Shift modifier is on, or if the Lock modifier is on
  and is interpreted as ShiftLock, then the first KeySym is used,
  otherwise the second KeySym is used.

* The Shift and Lock modifiers are both off. In this case, the first
  KeySym is used.

* The Shift modifier is off, and the Lock modifier is on and is
  interpreted as CapsLock. In this case, the first KeySym is used, but
  if that KeySym is lowercase alphabetic, then the corresponding
  uppercase KeySym is used instead.

* The Shift modifier is on, and the Lock modifier is on and is
  interpreted as CapsLock. In this case, the second KeySym is used, but
  if that KeySym is lowercase alphabetic, then the corresponding
  uppercase KeySym is used instead.

* The Shift modifier is on, or the Lock modifier is on and is
  interpreted as ShiftLock, or both. In this case, the second KeySym is
  used.

*/

xcb_keysym_t xcb_key_symbols_get_keysym (xcb_key_symbols_t *syms,
				  xcb_keycode_t     keycode,
				  int            col)
{
  xcb_keysym_t *keysyms;
  xcb_keysym_t  keysym_null = { 0 };
  xcb_keysym_t  lsym;
  xcb_keysym_t  usym;
  xcb_keycode_t min_keycode;
  xcb_keycode_t max_keycode;
  int        per;
  
  if (!syms)
    return keysym_null;
  
  xcb_key_symbols_get_reply (syms, NULL);
  
  keysyms = xcb_get_keyboard_mapping_keysyms (syms->u.reply);
  min_keycode = xcb_get_setup (syms->c)->min_keycode;
  max_keycode = xcb_get_setup (syms->c)->max_keycode;

  per = syms->u.reply->keysyms_per_keycode;
  if ((col < 0) || ((col >= per) && (col > 3)) ||
      (keycode.id < min_keycode.id) ||
      (keycode.id > max_keycode.id))
    return keysym_null;

  keysyms = &keysyms[(keycode.id - min_keycode.id) * per];
  if (col < 4)
    {
      if (col > 1)
	{
	  while ((per > 2) && (keysyms[per - 1].id == 0))
	    per--;
	  if (per < 3)
	    col -= 2;
	}
      if ((per <= (col|1)) || (keysyms[col|1].id == 0))
	{
	  xcb_convert_case(keysyms[col&~1], &lsym, &usym);
	  if (!(col & 1))
	    return lsym;
	  else if (usym.id == lsym.id)
	    return keysym_null;
	  else
	    return usym;
	}
    }
  return keysyms[col];
}


xcb_keycode_t
xcb_key_symbols_get_keycode (xcb_key_symbols_t *syms,
			 xcb_keysym_t      keysym)
{
  xcb_keysym_t    ks;
  xcb_keycode_t   keycode_null = { 0 };
  int          i, j;

  if (!syms)
    return keycode_null;
  
  xcb_key_symbols_get_reply (syms, NULL);

  for (j = 0; j < syms->u.reply->keysyms_per_keycode; j++)
    {
      for (i = xcb_get_setup (syms->c)->min_keycode.id; i <= xcb_get_setup (syms->c)->max_keycode.id; i++)
	{
	  xcb_keycode_t keycode;
	  
	  keycode.id = i;
	  ks = xcb_key_symbols_get_keysym (syms, keycode, j);
	  if (ks.id == keysym.id)
	    return keycode;
	}
    }
  
  return keycode_null;
}

xcb_keysym_t
xcb_key_press_lookup_keysym (xcb_key_symbols_t    *syms,
			 xcb_key_press_event_t *event,
			 int               col)
{
  return xcb_key_symbols_get_keysym (syms, event->detail, col);
}

xcb_keysym_t
xcb_key_release_lookup_keysym (xcb_key_symbols_t      *syms,
			   xcb_key_release_event_t *event,
			   int                 col)
{
  return xcb_key_symbols_get_keysym (syms, event->detail, col);
}

int
xcb_refresh_keyboard_mapping (xcb_key_symbols_t         *syms,
			   xcb_mapping_notify_event_t *event)
{
  if (event->request == XCB_MAPPING_KEYBOARD && syms) {
    if (syms->tag == TAG_VALUE) {
      xcb_keycode_t     min_keycode;
      xcb_keycode_t     max_keycode;

      if (syms->u.reply) {
	free (syms->u.reply);
	syms->u.reply = NULL;
      }
      syms->tag = TAG_COOKIE;
      min_keycode = xcb_get_setup (syms->c)->min_keycode;
      max_keycode = xcb_get_setup (syms->c)->max_keycode;
  
      syms->u.cookie = xcb_get_keyboard_mapping(syms->c,
					     min_keycode,
					     max_keycode.id - min_keycode.id + 1);
      
    }
    return 1;
  } 
  return 0;
}


/* Tests for classes of symbols */

int
xcb_is_keypad_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= XK_KP_Space) && (keysym.id <= XK_KP_Equal));
}

int
xcb_is_private_keypad_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= 0x11000000) && (keysym.id <= 0x1100FFFF));
}

int
xcb_is_cursor_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= XK_Home) && (keysym.id <= XK_Select));
}

int
xcb_is_pf_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= XK_KP_F1) && (keysym.id <= XK_KP_F4));
}

int
xcb_is_function_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= XK_F1) && (keysym.id <= XK_F35));
}

int
xcb_is_misc_function_key (xcb_keysym_t keysym)
{
  return ((keysym.id >= XK_Select) && (keysym.id <= XK_Break));
}

int
xcb_is_modifier_key (xcb_keysym_t keysym)
{
  return  (((keysym.id >= XK_Shift_L)  && (keysym.id <= XK_Hyper_R)) ||
	   ((keysym.id >= XK_ISO_Lock) && (keysym.id <= XK_ISO_Last_Group_Lock)) ||
	   (keysym.id == XK_Mode_switch) ||
	   (keysym.id == XK_Num_Lock));
}

/* private functions */

void
xcb_convert_case(xcb_keysym_t  sym,
	       xcb_keysym_t *lower,
	       xcb_keysym_t *upper)
{
  lower->id = sym.id;
  upper->id = sym.id;

  switch(sym.id >> 8)
    {
    case 0: /* Latin 1 */
      if ((sym.id >= XK_A) && (sym.id <= XK_Z))
	lower->id += (XK_a - XK_A);
      else if ((sym.id >= XK_a) && (sym.id <= XK_z))
	upper->id -= (XK_a - XK_A);
      else if ((sym.id >= XK_Agrave) && (sym.id <= XK_Odiaeresis))
	lower->id += (XK_agrave - XK_Agrave);
      else if ((sym.id >= XK_agrave) && (sym.id <= XK_odiaeresis))
	upper->id -= (XK_agrave - XK_Agrave);
      else if ((sym.id >= XK_Ooblique) && (sym.id <= XK_Thorn))
	lower->id += (XK_oslash - XK_Ooblique);
      else if ((sym.id >= XK_oslash) && (sym.id <= XK_thorn))
	upper->id -= (XK_oslash - XK_Ooblique);
      break;
    case 1: /* Latin 2 */
      /* Assume the KeySym.Id is a legal value (ignore discontinuities) */
      if (sym.id == XK_Aogonek)
	lower->id = XK_aogonek;
      else if (sym.id >= XK_Lstroke && sym.id <= XK_Sacute)
	lower->id += (XK_lstroke - XK_Lstroke);
      else if (sym.id >= XK_Scaron && sym.id <= XK_Zacute)
	lower->id += (XK_scaron - XK_Scaron);
      else if (sym.id >= XK_Zcaron && sym.id <= XK_Zabovedot)
	lower->id += (XK_zcaron - XK_Zcaron);
      else if (sym.id == XK_aogonek)
	upper->id = XK_Aogonek;
      else if (sym.id >= XK_lstroke && sym.id <= XK_sacute)
	upper->id -= (XK_lstroke - XK_Lstroke);
      else if (sym.id >= XK_scaron && sym.id <= XK_zacute)
	upper->id -= (XK_scaron - XK_Scaron);
      else if (sym.id >= XK_zcaron && sym.id <= XK_zabovedot)
	upper->id -= (XK_zcaron - XK_Zcaron);
      else if (sym.id >= XK_Racute && sym.id <= XK_Tcedilla)
	lower->id += (XK_racute - XK_Racute);
      else if (sym.id >= XK_racute && sym.id <= XK_tcedilla)
	upper->id -= (XK_racute - XK_Racute);
      break;
    case 2: /* Latin 3 */
      /* Assume the KeySym.Id is a legal value (ignore discontinuities) */
      if (sym.id >= XK_Hstroke && sym.id <= XK_Hcircumflex)
	lower->id += (XK_hstroke - XK_Hstroke);
      else if (sym.id >= XK_Gbreve && sym.id <= XK_Jcircumflex)
	lower->id += (XK_gbreve - XK_Gbreve);
      else if (sym.id >= XK_hstroke && sym.id <= XK_hcircumflex)
	upper->id -= (XK_hstroke - XK_Hstroke);
      else if (sym.id >= XK_gbreve && sym.id <= XK_jcircumflex)
	upper->id -= (XK_gbreve - XK_Gbreve);
      else if (sym.id >= XK_Cabovedot && sym.id <= XK_Scircumflex)
	lower->id += (XK_cabovedot - XK_Cabovedot);
      else if (sym.id >= XK_cabovedot && sym.id <= XK_scircumflex)
	upper->id -= (XK_cabovedot - XK_Cabovedot);
      break;
    case 3: /* Latin 4 */
      /* Assume the KeySym.Id is a legal value (ignore discontinuities) */
      if (sym.id >= XK_Rcedilla && sym.id <= XK_Tslash)
	lower->id += (XK_rcedilla - XK_Rcedilla);
      else if (sym.id >= XK_rcedilla && sym.id <= XK_tslash)
	upper->id -= (XK_rcedilla - XK_Rcedilla);
      else if (sym.id == XK_ENG)
	lower->id = XK_eng;
      else if (sym.id == XK_eng)
	upper->id = XK_ENG;
      else if (sym.id >= XK_Amacron && sym.id <= XK_Umacron)
	lower->id += (XK_amacron - XK_Amacron);
      else if (sym.id >= XK_amacron && sym.id <= XK_umacron)
	upper->id -= (XK_amacron - XK_Amacron);
      break;
    case 6: /* Cyrillic */
      /* Assume the KeySym.Id is a legal value (ignore discontinuities) */
      if (sym.id >= XK_Serbian_DJE && sym.id <= XK_Serbian_DZE)
	lower->id -= (XK_Serbian_DJE - XK_Serbian_dje);
      else if (sym.id >= XK_Serbian_dje && sym.id <= XK_Serbian_dze)
	upper->id += (XK_Serbian_DJE - XK_Serbian_dje);
      else if (sym.id >= XK_Cyrillic_YU && sym.id <= XK_Cyrillic_HARDSIGN)
	lower->id -= (XK_Cyrillic_YU - XK_Cyrillic_yu);
      else if (sym.id >= XK_Cyrillic_yu && sym.id <= XK_Cyrillic_hardsign)
	upper->id += (XK_Cyrillic_YU - XK_Cyrillic_yu);
      break;
    case 7: /* Greek */
      /* Assume the KeySym.Id is a legal value (ignore discontinuities) */
      if (sym.id >= XK_Greek_ALPHAaccent && sym.id <= XK_Greek_OMEGAaccent)
	lower->id += (XK_Greek_alphaaccent - XK_Greek_ALPHAaccent);
      else if (sym.id >= XK_Greek_alphaaccent && sym.id <= XK_Greek_omegaaccent &&
	       sym.id != XK_Greek_iotaaccentdieresis &&
	       sym.id != XK_Greek_upsilonaccentdieresis)
	upper->id -= (XK_Greek_alphaaccent - XK_Greek_ALPHAaccent);
      else if (sym.id >= XK_Greek_ALPHA && sym.id <= XK_Greek_OMEGA)
	lower->id += (XK_Greek_alpha - XK_Greek_ALPHA);
      else if (sym.id >= XK_Greek_alpha && sym.id <= XK_Greek_omega &&
	       sym.id != XK_Greek_finalsmallsigma)
	upper->id -= (XK_Greek_alpha - XK_Greek_ALPHA);
      break;
    case 0x14: /* Armenian */
      if (sym.id >= XK_Armenian_AYB && sym.id <= XK_Armenian_fe) {
	lower->id = sym.id | 1;
	upper->id = sym.id & ~1;
      }
      break;
    }
}

void
xcb_key_symbols_get_reply (xcb_key_symbols_t    *syms,
		       xcb_generic_error_t **e)
{
  if (!syms)
    return;

  if (syms->tag == TAG_COOKIE)
    {
      syms->tag = TAG_VALUE;
      syms->u.reply = xcb_get_keyboard_mapping_reply(syms->c,
						 syms->u.cookie,
						 e);
    }
}
