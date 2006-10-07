#ifndef __XCB_REPLY_H__
#define __XCB_REPLY_H__

#include <xcb/xcb.h>
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct reply_handlers reply_handlers_t;
reply_handlers_t *alloc_reply_handlers(xcb_connection_t *c);
void free_reply_handlers(reply_handlers_t *h);
xcb_connection_t *get_xcb_connection(reply_handlers_t *h);

int poll_replies(reply_handlers_t *h);
void start_reply_thread(reply_handlers_t *h);
void stop_reply_thread(reply_handlers_t *h);

typedef void (*generic_reply_handler)(void *data, xcb_connection_t *c, xcb_generic_reply_t *reply, xcb_generic_error_t *error);

void add_reply_handler(reply_handlers_t *h, unsigned int request, generic_reply_handler handler, void *data);


#ifdef __cplusplus
}
#endif


#endif /* __XCB_REPLY_H__ */
