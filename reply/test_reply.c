#include "xcb_reply.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fontinfo_handler(void *data, XCBConnection *c, XCBGenericRep *rg, XCBGenericError *eg)
{
	XCBListFontsWithInfoRep *rep = (XCBListFontsWithInfoRep *) rg;
	if(rep)
	{
		int len = XCBListFontsWithInfoNameLength(rep);
		if(len)
			printf("(+%u) Font \"%.*s\"\n",
					(unsigned int) rep->replies_hint,
					len, XCBListFontsWithInfoName(rep));
		else
			printf("End of font list.\n");
	}
	if(eg)
		printf("Error from ListFontsWithInfo: %d\n", eg->error_code);
}

int main(int argc, char **argv)
{
	int count = 10;
	char *pattern = "*";
	XCBConnection *c = XCBConnect(NULL, NULL);
	ReplyHandlers *h = allocReplyHandlers(c);
	pthread_t reply_thread;

	if(argc > 1)
		count = atoi(argv[1]);
	if(argc > 2)
		pattern = argv[2];
	
	AddReplyHandler(h, XCBListFontsWithInfo(c, count, strlen(pattern), pattern).sequence, fontinfo_handler, 0);
	reply_thread = StartReplyThread(h);

	XCBSync(c, 0);
	StopReplyThreads(h);
	pthread_join(reply_thread, 0);
	XCBDisconnect(c);
	exit(0);
}
