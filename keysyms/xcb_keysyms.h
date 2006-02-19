#ifndef __XCB_KEYSYMS_H__
#define __XCB_KEYSYMS_H__



typedef struct _XCBKeySymbols XCBKeySymbols;


XCBKeySymbols *XCBKeySymbolsAlloc        (XCBConnection         *c);

void           XCBKeySymbolsFree         (XCBKeySymbols         *syms);

XCBKEYSYM      XCBKeySymbolsGetKeysym    (XCBKeySymbols         *syms,
					  XCBKEYCODE             keycode,
					  int                    col);

XCBKEYCODE     XCBKeySymbolsGetKeycode   (XCBKeySymbols         *syms,
					  XCBKEYSYM              keysym);

XCBKEYSYM      XCBKeyPressLookupKeysym   (XCBKeySymbols         *syms,
					  XCBKeyPressEvent      *event,
					  int                    col);

XCBKEYSYM      XCBKeyReleaseLookupKeysym (XCBKeySymbols         *syms,
					  XCBKeyReleaseEvent    *event,
					  int                    col);

int            XCBRefreshKeyboardMapping (XCBKeySymbols         *syms,
					  XCBMappingNotifyEvent *event);


/* Tests for classes of symbols */

int XCBIsKeypadKey        (XCBKEYSYM keysym);

int XCBIsPrivateKeypadKey (XCBKEYSYM keysym);

int XCBIsCursorKey        (XCBKEYSYM keysym);

int XCBIsPFKey            (XCBKEYSYM keysym);

int XCBIsFunctionKey      (XCBKEYSYM keysym);

int XCBIsMiscFunctionKey  (XCBKEYSYM keysym);

int XCBIsModifierKey      (XCBKEYSYM keysym);


#endif /* __XCB_KEYSYMS_H__ */
