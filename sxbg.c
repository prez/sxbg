/* see LICENSE file for copyright and license details */
#include <err.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xcb_aux.h>

enum {
	W = 1920,
	H = 1080,
};

int
main(int argc, char **argv)
{
	char *hex = argv[1];
	if (2 != argc || !(7 == strlen(hex) || 9 == strlen(hex))) {
		fprintf(stderr, "usage: sxbg \"#RRGGBB[AA]\"\n");
		return EXIT_FAILURE;
	}

	uint16_t rgb[3];
	for (size_t i = 1; i < 7; i += 2) {
		char *tmp;
		char str[] = { hex[i], hex[i+1], '\0' };
		rgb[(i-1)/2] = ((uint16_t)strtoul(
			str,
			&tmp,
			16) & 0xFF) * 257;
		if (tmp == str || *tmp != '\0')
			errx(EXIT_FAILURE, "failed to parse the colour %s", str);
	}

	int scrp;
	xcb_connection_t *conn = xcb_connect(NULL, &scrp);
	if (xcb_connection_has_error(conn))
		errx(EXIT_FAILURE, "unable to connect to the X server");

	xcb_screen_t *scr = xcb_aux_get_screen(conn, scrp);
	if (NULL == scr)
		errx(EXIT_FAILURE, "unable to access the X server screen");
	xcb_window_t root = scr->root;
	xcb_colormap_t colormap = scr->default_colormap;

	xcb_alloc_color_reply_t *repl = xcb_alloc_color_reply(
		conn,
		xcb_alloc_color(conn, colormap, rgb[0], rgb[1], rgb[2]),
		NULL);
	if (NULL == repl)
		errx(EXIT_FAILURE, "failed to allocate color");
	uint32_t params[1] = { repl->pixel };
	free(repl);

	xcb_void_cookie_t res = xcb_change_window_attributes(
		conn,
		root,
		XCB_CW_BACK_PIXEL,
		params);
	xcb_generic_error_t *e = xcb_request_check(conn, res);
	if (e)
		errx(EXIT_FAILURE, "failed to set background");

	xcb_clear_area(conn, 1, root, 0, 0, W, H);
	xcb_aux_sync(conn);
	xcb_disconnect(conn);
}
