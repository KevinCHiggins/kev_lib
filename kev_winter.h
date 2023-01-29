#ifndef _KEV_WINTER_HEADER_
#define _KEV_WINTER_HEADER_
#include <stdint.h>

/*
struct point
{
	size_t x;
	size_t y;
};
typedef struct point point;

struct point_float
{
	float x;
	float y;
};
typedef struct point_float point_float;
*/

#ifdef _WIN32
#include <windows.h>
#endif

struct kev_win{
	int width;
	int height;
	char *title;
	uint32_t *buffer;
#ifdef _WIN32
	HWND handle;
#endif
};

typedef struct kev_win kev_win;


unsigned int rgb(unsigned char r, unsigned char g, unsigned char b);


void poll_event(kev_win *win);
void redraw();
void init(kev_win *win);


	
#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
Display *dis;
int screen;
Window win;
GC gc;
KeySym key;

XImage *buff_ximage;
Visual vis;

XImage set_ximage();
void close_x11();

#endif
#endif