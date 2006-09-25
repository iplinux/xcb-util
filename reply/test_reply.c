#include <xcb/xcb.h>
#include "xcb_reply.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fontinfo_handler(void *data, xcb_connection_t *c, xcb_generic_reply_t *rg, xcb_generic_error_t *eg)
{
	xcb_list_fonts_with_info_reply_t *rep = (xcb_list_fonts_with_info_reply_t *) rg;
	if(rep)
	{
		int len = xcb_list_fonts_with_info_name_length(rep);
		if(len)
			printf("(+%u) Font \"%.*s\"\n",
					(unsigned int) rep->replies_hint,
					len, xcb_list_fonts_with_info_name(rep));
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
	xcb_connection_t *c = xcb_connect(NULL, NULL);
	reply_handlers_t *h = alloc_reply_handlers(c);
	pthread_t reply_thread;

	if(argc > 1)
		count = atoi(argv[1]);
	if(argc > 2)
		pattern = argv[2];
	
	add_reply_handler(h, xcb_list_fonts_with_info(c, count, strlen(pattern), pattern).sequence, fontinfo_handler, 0);
	reply_thread = start_reply_thread(h);

	free(xcb_get_input_focus_reply(c, xcb_get_input_focus(c), NULL));
	stop_reply_threads(h);
	pthread_join(reply_thread, 0);
	xcb_disconnect(c);
	exit(0);
}
