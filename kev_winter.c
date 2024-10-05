#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kev_winter.h"


void kev_win_drain_events(kev_win *win);

bool is_pressed[100]; // doesn't handle focus for multiple windows

#ifdef _WIN32
#include <windows.h>
const char class_name[] = "kev_winter";

kev_win_event_list_item *event_list_end;
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
void kev_win_update_events(kev_win *win)
{
	kev_win_drain_events(win); // in case client didn't poll events

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
	break;
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
	case WM_KEYDOWN:
		{
			if (uint_param)
			{
				enum Keycode kev_win_keycode = windows_virtual_keycodes_map[uint_param];
				win->event_list_start = kev_win_queue_new_event(KEYPRESS, kev_win_keycode);
				is_pressed[kev_win_keycode] = true;
				printf("Win: %u, kev_win: %d to %d\n", uint_param, kev_win_keycode, is_pressed[kev_win_keycode]);
				printf("Keycode UP DOWN LEFT RIGHT %d %d %d %d \n", KEYCODE_UP, KEYCODE_DOWN, KEYCODE_LEFT, KEYCODE_RIGHT);

			}
		}
	break;
	case WM_KEYUP:
		{
			if (uint_param)
			{
				enum Keycode kev_win_keycode = windows_virtual_keycodes_map[uint_param];
				win->event_list_start = kev_win_queue_new_event(KEYRELEASE, kev_win_keycode);
				is_pressed[kev_win_keycode] = false;
				printf("Is released %d\n", kev_win_keycode);
			}
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
	win->scaled_buffer = calloc(1, sizeof(uint32_t) * win->height * win->width * win->scale * win->scale);


	printf("w %d orig %d scale %d", win->width * win->scale, win->width, win->scale);

	win->x_win = XCreateSimpleWindow(win->dis, DefaultRootWindow(win->dis), 0, 0, win->width * win->scale, win->height*win->scale, 5, 0, 0);
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
	free(win->scaled_buffer);
	//exit(0);
}
void kev_win_update_events(kev_win *win)
{
	redraw(win);
	kev_win_drain_events(win); // in case client didn't poll events

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
		else if (event.type == KeyPress)
		{
			int kev_win_keycode = linux_keycodes_map[event.xkey.keycode];
			is_pressed[kev_win_keycode] = true;
			win->event_list_start = kev_win_queue_new_event(KEYPRESS, kev_win_keycode);
			printf("Linux: %d, kev_win: %d to %d\n", event.xkey.keycode, kev_win_keycode, is_pressed[kev_win_keycode]);
			printf("Keycode S X ENTER \\ CTRL %d %d %d %d %d\n", KEYCODE_S, KEYCODE_X, KEYCODE_ENTER, KEYCODE_BACKSLASH, KEYCODE_CTRL);
		}
		else if (event.type == KeyRelease)
		{
			enum Keycode kev_win_keycode = linux_keycodes_map[event.xkey.keycode];
			is_pressed[kev_win_keycode] = false;
			win->event_list_start = kev_win_queue_new_event(KEYRELEASE, kev_win_keycode);
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
	for (int row = 0; row < win->height; row++)
	{
		for (int pixel = 0; pixel < win->width; pixel++)
		{
			uint32_t pixel_value = win->buffer[row * win->width + pixel];

			for (int y_scale = 0; y_scale < win->scale; y_scale++)
			{
				for (int x_scale = 0; x_scale < win->scale; x_scale++)
				{
					win->scaled_buffer[((row * win->scale + y_scale) * win->width * win->scale) + ((pixel * win->scale) + x_scale)] = pixel_value;
				}
			}
			
		}
	}

	if (!win->buff_ximage)
	{
		win->buff_ximage = XCreateImage(win->dis, DefaultVisual(win->dis, win->screen), 24, ZPixmap, 0, (char *)win->scaled_buffer, win->width * win->scale, win->height * win->scale, 32, 0);
		XInitImage(win->buff_ximage);
	}

	XPutImage(win->dis, win->x_win, win->gc, win->buff_ximage, 0, 0, 0, 0, win->width * win->scale, win->height * win->scale);
	//XMapWindow(win->dis, win->x_win);

	//XClearWindow(dis, win);
}

#endif

kev_win_event_list_item *kev_win_queue_new_event(int type, int keycode)
{
	kev_win_event_list_item *ptr = (kev_win_event_list_item *)calloc(1, sizeof(kev_win_event_list_item));
	ptr->event.type = type;
	ptr->event.keycode = keycode;
}

int kev_win_poll_event(kev_win *win, kev_win_event *event)
{
	if (win->event_list_start)
	{
		kev_win_event_list_item *start = win->event_list_start;
		*event = start->event;

		win->event_list_start = win->event_list_start->next;
		free(start);
		return 1;
	}
	return 0;
}

void kev_win_drain_events(kev_win *win)
{
		kev_win_event event;
		int event_found = kev_win_poll_event(win, &event);
		while (event_found) event_found = kev_win_poll_event(win, &event);
}

bool kev_win_is_pressed(int keycode)
{
	return is_pressed[keycode];
}