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

unsigned int rgb(unsigned char r, unsigned char g, unsigned char b);

#ifndef _WINTER_HEADER_
	#define _WINTER_HEADER_
	extern int w;
	extern int h;
	void poll_event();
	void redraw();
	void init();
	#ifdef _WIN32
		#include <windows.h>
		WNDCLASSEX window_class;
		HWND handle;
		MSG msg;
		COLORREF purple;
		HBRUSH purple_brush;
		HINSTANCE main_inst;
		extern const char class_name[];
		LRESULT CALLBACK window_proc(HWND handle, UINT msg, WPARAM uint_param, LPARAM long_param);
	#endif
	
	#ifdef __linux__
		#include <X11/Xlib.h>
		#include <X11/Xutil.h>
		#include <X11/Xos.h>
		Display *dis;
		int screen;
		Window win;
		GC gc;
		XEvent event;
		KeySym key;
		long event_mask;
		XImage *buff_ximage;
		Visual vis;

		XImage set_ximage();
		void close_x11();
		void redraw();
	#endif
#endif