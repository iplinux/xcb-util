#ifndef __XCB_REPLY_H__
#define __XCB_REPLY_H__

#include <X11/XCB/xcb.h>
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct ReplyHandlers ReplyHandlers;
ReplyHandlers *allocReplyHandlers(XCBConnection *c);
void freeReplyHandlers(ReplyHandlers *h);
XCBConnection *getXCBConnection(ReplyHandlers *h);

int PollReplies(ReplyHandlers *h);
pthread_t StartReplyThread(ReplyHandlers *h);
void StopReplyThreads(ReplyHandlers *h);

typedef void (*GenericReplyHandler)(void *data, XCBConnection *c, XCBGenericRep *reply, XCBGenericError *error);

void AddReplyHandler(ReplyHandlers *h, unsigned int request, GenericReplyHandler handler, void *data);


#ifdef __cplusplus
}
#endif


#endif /* __XCB_REPLY_H__ */
