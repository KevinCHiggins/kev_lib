#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kev_winter.h"

#ifdef _WIN32
#include <windows.h>
const char class_name[] = "kev_winter";

HDC hdc;
BITMAPINFO buff_info;
int64_t get_tick_in_ns();
void reset_buffer_info(kev_win *win);
void redraw(kev_win *win);
LRESULT CALLBACK window_proc(HWND handle, UINT msg, WPARAM uint_param, LPARAM long_param);


void kev_win_init(kev_win *win)
{


	WNDCLASSEX window_class; // = {0};
	memset(&window_class, 0, sizeof(window_class));
	HINSTANCE instance = GetModuleHandle(NULL);
	window_class.cbSize		= sizeof(WNDCLASSEX);
	window_class.style		= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc	= window_proc;
	window_class.hInstance		= instance;
	window_class.lpszClassName	= class_name;
	
	if (!RegisterClassEx(&window_class))
	{
		MessageBox(NULL, "Window couldn't be registered", "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(1); // or should it be zero? A quick look online suggests it's up to me
	}

	int top = 100;
	int left = 100;
	RECT rect;
	rect.left   = left;
	rect.top    = top;
	rect.right  = left + win->width;
	rect.bottom = top + win->height;

	UINT style = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&rect, style, 0, 0);
	rect.right += 3; // to show the full client area on Win10
	rect.bottom += 3; // to show the full client area on Win10

	HWND handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		class_name,
		win->title,
		WS_OVERLAPPEDWINDOW,
		rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
		NULL, NULL, instance, NULL);

	if (handle == NULL)
	{
		MessageBox(NULL, "Window couldn't be created", "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(1);	
	}

	SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR)win);
	win->handle = handle;
	
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
	hdc = GetDC(handle);

	reset_buffer_info(win);
	redraw(win);

}
void kev_win_poll_event(kev_win *win)
{

	MSG msg;
	if (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		exit(msg.wParam);
	}

	
}
void reset_buffer_info(kev_win *win)
{

	buff_info.bmiHeader.biSize = sizeof(buff_info.bmiHeader);
	buff_info.bmiHeader.biWidth = win->width;
	buff_info.bmiHeader.biHeight = 0 - win->height;
	buff_info.bmiHeader.biPlanes = 1;
	buff_info.bmiHeader.biBitCount = 32;
	buff_info.bmiHeader.biCompression = BI_RGB;
	buff_info.bmiHeader.biSizeImage = 0;
	
}
void redraw(kev_win *win)
{
	//printf("Redrawering...\n");
	//printf("%d", win->buffer[0]);
	StretchDIBits(hdc, 0, 0, win->width, win->height, 0, 0,win->width, win->height, win->buffer, &buff_info, DIB_RGB_COLORS, SRCCOPY);


}

// pointer to either __int64 (64-bit) or long, __stdcall
LRESULT CALLBACK window_proc(HWND handle, UINT msg, WPARAM uint_param, LPARAM long_param)
{
kev_win *win = (kev_win*)GetWindowLongPtr(handle, GWLP_USERDATA);

	switch(msg)
	{
	case WM_CLOSE:
		{
		DestroyWindow(handle);
		}

	break;
	case WM_DESTROY:
		{

		PostQuitMessage(0);
		}
	break;
	case WM_SIZE:
		{

		redraw(win);
		}
	case WM_MOUSEMOVE:
		{

		redraw(win);
		}
	break;
	case WM_PAINT:
		{
		redraw(win);

		}
	break;
	default:
		return DefWindowProc(handle, msg, uint_param, long_param);
	}
	return 0;
}

#endif


#ifdef __linux__

void close_x11();
void redraw();

Atom wm_delete;
const long event_mask = StructureNotifyMask | ExposureMask | SubstructureNotifyMask;


void kev_win_init(kev_win *win)
{

	win->dis = XOpenDisplay((char *)0);
	

	if (win->dis == NULL)
	{
		printf("Cannot open display\n");
		exit(1);
	}

	win->screen = DefaultScreen(win->dis);




	win->x_win = XCreateSimpleWindow(win->dis, DefaultRootWindow(win->dis), 0, 0, win->width, win->height, 5, 0, 0);
	XSetStandardProperties(win->dis, win->x_win, win->title, "Yo", None, NULL, 0, NULL);
	Atom *protocols_list;
	int num_protocols;

	wm_delete = XInternAtom(win->dis, "WM_TAKE_FOCUS", True);
	if (!XSetWMProtocols(win->dis, win->x_win, &wm_delete, 1))
	{
		printf("Cannot set delete window protoocl");
	}
	if (XGetWMProtocols(win->dis, win->x_win, &protocols_list, &num_protocols))
	{
		printf("Got %d protocols", num_protocols);

	}
	else
	{
		printf("Failed to get protocols");
	}
	win->gc = XCreateGC(win->dis, win->x_win, 0, 0);
	XStoreName(win->dis, win->x_win, win->title);
	XSync(win->dis, False);

	//XWindowAttributes attr;
	//XGetWindowAttributes(win->dis, win->x_win, &attr);
	XSelectInput(win->dis, win->x_win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask |
                   ButtonReleaseMask | PointerMotionMask);

	XClearWindow(win->dis,win->x_win);
	XMapRaised(win->dis, win->x_win);
	redraw(win);



}
void close_x11(kev_win *win)
{
	printf("Exiting");
	XFreeGC(win->dis, win->gc);
	XDestroyWindow(win->dis, win->x_win);
	XCloseDisplay(win->dis);
	//exit(0);
}
void kev_win_poll_event(kev_win *win)
{
	redraw(win);

	XEvent event;
	//printf("%ld", LastKnownRequestProcessed(dis));
	XFlush(win->dis);
	//if (XPending(win->dis)) printf("P: %d", XPending(win->dis));
	//printf("%d", event.type);
	while (XPending(win->dis))
	{
		XNextEvent(win->dis, &event);
		//printf("Event %d\n", event.type);
		if (event.type == Expose)
		{
			
		}
	}
	if (XCheckWindowEvent(win->dis, win->x_win, NoEventMask, &event)) printf("Erm");
	while (XCheckWindowEvent(win->dis, win->x_win, NoEventMask, &event))

	{
		printf("Er");
		if (event.type) printf("E;");
		
		if (event.type == ConfigureNotify)
		{
			printf("Thing");
			exit(1);
			redraw();
		}

		else if (event.type == DestroyNotify)
		{
			printf("Funk");
		}
		else if (event.type == ClientMessage)
		{
			printf("Client message");
			if (event.xclient.data.l[0] = wm_delete)
			{
				close_x11(win);
			}
		}
		
	}

}
void redraw(kev_win *win)

{

	if (!win->buff_ximage)
	{
		win->buff_ximage = XCreateImage(win->dis, DefaultVisual(win->dis, win->screen), 24, ZPixmap, 0, (char *)win->buffer, win->width, win->height, 32, 0);
		XInitImage(win->buff_ximage);
	}

	XPutImage(win->dis, win->x_win, win->gc, win->buff_ximage, 0, 0, 0, 0, win->width, win->height);
	//XMapWindow(win->dis, win->x_win);
	//XClearWindow(dis, win);
}

#endif
