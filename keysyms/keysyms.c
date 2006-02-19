#include <malloc.h>

#include <X11/XCB/xcb.h>
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
  XCBConnection *c;
  enum tag_t     tag;
  union {
    XCBGetKeyboardMappingCookie cookie;
    XCBGetKeyboardMappingRep *reply;
  } u;
};

static void XCBConvertCase(XCBKEYSYM  sym,
			   XCBKEYSYM *lower,
			   XCBKEYSYM *upper);

static void XCBKeySymbolsGetReply (XCBKeySymbols    *syms,
				   XCBGenericError **e);

/* public implementation */

XCBKeySymbols *
XCBKeySymbolsAlloc (XCBConnection *c)
{
  XCBKeySymbols *syms;
  XCBKEYCODE     min_keycode;
  XCBKEYCODE     max_keycode;

  if (!c)
    return NULL;

  syms = (XCBKeySymbols *)malloc (sizeof (XCBKeySymbols));

  syms->c = c;
  syms->tag = TAG_COOKIE;

  min_keycode = XCBGetSetup (c)->min_keycode;
  max_keycode = XCBGetSetup (c)->max_keycode;
  
  syms->u.cookie = XCBGetKeyboardMapping(c,
					 min_keycode,
					 max_keycode.id - min_keycode.id + 1);
  
  return syms;
}

void
XCBKeySymbolsFree (XCBKeySymbols *syms)
{
  if (syms)
    {
      if (syms->tag == TAG_VALUE)
	free (syms->u.reply);
      free (syms);
      syms = NULL;
    }
}

XCBKEYSYM XCBKeySymbolsGetKeysym (XCBKeySymbols *syms,
				  XCBKEYCODE     keycode,
				  int            col)
{
  XCBKEYSYM *keysyms;
  XCBKEYSYM  keysym_null = { 0 };
  XCBKEYSYM  lsym;
  XCBKEYSYM  usym;
  XCBKEYCODE min_keycode;
  XCBKEYCODE max_keycode;
  int        per;
  
  if (!syms)
    return keysym_null;
  
  XCBKeySymbolsGetReply (syms, NULL);
  
  keysyms = XCBGetKeyboardMappingKeysyms (syms->u.reply);
  min_keycode = XCBGetSetup (syms->c)->min_keycode;
  max_keycode = XCBGetSetup (syms->c)->max_keycode;

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
	  XCBConvertCase(keysyms[col&~1], &lsym, &usym);
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


XCBKEYCODE
XCBKeySymbolsGetKeycode (XCBKeySymbols *syms,
			 XCBKEYSYM      keysym)
{
  XCBKEYSYM    ks;
  XCBKEYCODE   keycode_null = { 0 };
  int          i, j;

  if (!syms)
    return keycode_null;
  
  XCBKeySymbolsGetReply (syms, NULL);

  for (j = 0; j < syms->u.reply->keysyms_per_keycode; j++)
    {
      for (i = XCBGetSetup (syms->c)->min_keycode.id; i <= XCBGetSetup (syms->c)->max_keycode.id; i++)
	{
	  XCBKEYCODE keycode;
	  
	  keycode.id = i;
	  ks = XCBKeySymbolsGetKeysym (syms, keycode, j);
	  if (ks.id == keysym.id)
	    return keycode;
	}
    }
  
  return keycode_null;
}

XCBKEYSYM
XCBKeyPressLookupKeysym (XCBKeySymbols    *syms,
			 XCBKeyPressEvent *event,
			 int               col)
{
  return XCBKeySymbolsGetKeysym (syms, event->detail, col);
}

XCBKEYSYM
XCBKeyReleaseLookupKeysym (XCBKeySymbols      *syms,
			   XCBKeyReleaseEvent *event,
			   int                 col)
{
  return XCBKeySymbolsGetKeysym (syms, event->detail, col);
}

int
XCBRefreshKeyboardMapping (XCBKeySymbols         *syms,
			   XCBMappingNotifyEvent *event)
{
  if (event->request == MappingKeyboard && syms) {
    if (syms->tag == TAG_VALUE) {
      XCBKEYCODE     min_keycode;
      XCBKEYCODE     max_keycode;

      if (syms->u.reply) {
	free (syms->u.reply);
	syms->u.reply = NULL;
      }
      syms->tag = TAG_COOKIE;
      min_keycode = XCBGetSetup (syms->c)->min_keycode;
      max_keycode = XCBGetSetup (syms->c)->max_keycode;
  
      syms->u.cookie = XCBGetKeyboardMapping(syms->c,
					     min_keycode,
					     max_keycode.id - min_keycode.id + 1);
      
    }
    return 1;
  } 
  return 0;
}


/* Tests for classes of symbols */

int
XCBIsKeypadKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= XK_KP_Space) && (keysym.id <= XK_KP_Equal));
}

int
XCBIsPrivateKeypadKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= 0x11000000) && (keysym.id <= 0x1100FFFF));
}

int
XCBIsCursorKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= XK_Home) && (keysym.id <= XK_Select));
}

int
XCBIsPFKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= XK_KP_F1) && (keysym.id <= XK_KP_F4));
}

int
XCBIsFunctionKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= XK_F1) && (keysym.id <= XK_F35));
}

int
XCBIsMiscFunctionKey (XCBKEYSYM keysym)
{
  return ((keysym.id >= XK_Select) && (keysym.id <= XK_Break));
}

int
XCBIsModifierKey (XCBKEYSYM keysym)
{
  return  (((keysym.id >= XK_Shift_L)  && (keysym.id <= XK_Hyper_R)) ||
	   ((keysym.id >= XK_ISO_Lock) && (keysym.id <= XK_ISO_Last_Group_Lock)) ||
	   (keysym.id == XK_Mode_switch) ||
	   (keysym.id == XK_Num_Lock));
}

/* private functions */

void
XCBConvertCase(XCBKEYSYM  sym,
	       XCBKEYSYM *lower,
	       XCBKEYSYM *upper)
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
XCBKeySymbolsGetReply (XCBKeySymbols    *syms,
		       XCBGenericError **e)
{
  if (!syms)
    return;

  if (syms->tag == TAG_COOKIE)
    {
      syms->tag = TAG_VALUE;
      syms->u.reply = XCBGetKeyboardMappingReply(syms->c,
						 syms->u.cookie,
						 e);
    }
}
