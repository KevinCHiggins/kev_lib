#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "kev_winter.h"

#define WIDTH 320
#define HEIGHT 240

uint32_t buff[WIDTH * HEIGHT];

#ifdef _WIN32
#include <windows.h>
const char class_name[] = "True and Living";

HDC hdc;
BITMAPINFO buff_info;
void reset_buffer_info();

// note I could just use the global msg variable to the same effect
// probably a sign I should refactor
// Also, this method is not in the header file for the moment,
// as so far only poll_event should be quitting
void close_and_exit(MSG quit_msg)
{
	exit(quit_msg.wParam);
}
void init()
{
	
	window_class.cbSize		= sizeof(WNDCLASSEX);
	window_class.style		= CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc	= window_proc;
	window_class.hInstance		= main_inst;
	window_class.lpszClassName	= class_name;
	
	if (!RegisterClassEx(&window_class))
	{
		MessageBox(NULL, "Window couldn't be registered", "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(1); // or should it be zero? A quick look online suggests it's up to me
	}

	handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		class_name,
		"Tyrian Purple",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
		NULL, NULL, main_inst, NULL);

	if (handle == NULL)
	{
		MessageBox(NULL, "Window couldn't be created", "Error", MB_ICONEXCLAMATION | MB_OK);
		exit(1);	
	}
	
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
	hdc = GetDC(handle);
	reset_buffer_info();
}
void poll_event()
{
	// we don't specify the window handle because doing so means the quit message won't be received
	if (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		close_and_exit(msg);
	}

	
}
void reset_buffer_info()
{

	buff_info.bmiHeader.biSize = sizeof(buff_info.bmiHeader);
	buff_info.bmiHeader.biWidth = WIDTH;
	buff_info.bmiHeader.biHeight = 0 - HEIGHT;
	buff_info.bmiHeader.biPlanes = 1;
	buff_info.bmiHeader.biBitCount = 32;
	buff_info.bmiHeader.biCompression = BI_RGB;
	buff_info.bmiHeader.biSizeImage = 0;
	
}
void redraw()
{

	StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buff, &buff_info, DIB_RGB_COLORS, SRCCOPY);

}

// pointer to either __int64 (64-bit) or long, __stdcall
LRESULT CALLBACK window_proc(HWND handle, UINT msg, WPARAM uint_param, LPARAM long_param)
{
	switch(msg)
	{
	case WM_CLOSE:
		{
		DestroyWindow(handle);
		DeleteObject(purple_brush);
		}

	break;
	case WM_DESTROY:
		{

		PostQuitMessage(0);
		}
	break;
	case WM_SIZE:
		{
			redraw();

		}
	break;
	default:
		return DefWindowProc(handle, msg, uint_param, long_param);
	}
	return 0;
}

#endif

#ifdef __linux__

Atom wm_delete;

void init()
{

	dis = XOpenDisplay((char *)0);
	
	event_mask = StructureNotifyMask | ExposureMask;
	if (dis == NULL)
	{
		printf("Cannot open display\n");
		exit(1);
	}
	wm_delete = XInternAtom(dis, "WM_TAKE_FOCUS", True);
	screen = DefaultScreen(dis);

	win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, WIDTH, HEIGHT, 5, 0, 0);
	XSetStandardProperties(dis, win, "Tyrian Purple", "Yo", None, NULL, 0, NULL);
	Atom *protocols_list;
	int num_protocols;


	if (!XSetWMProtocols(dis, win, &wm_delete, 1))
	{
		printf("Cannot set delete window protoocl");
	}
	if (XGetWMProtocols(dis, win, &protocols_list, &num_protocols))
	{
		printf("Got %d protocols", num_protocols);

	}
	else
	{
		printf("Failed to get protocols");
	}
	gc = XCreateGC(dis, win, 0, 0);
	XSync(dis, False);

	XWindowAttributes attr;
	XGetWindowAttributes(dis, win, &attr);

	XClearWindow(dis, win);
	XMapRaised(dis, win);

}
void close_x11()
{
	printf("Exiting");
	XFreeGC(dis, gc);
	XDestroyWindow(dis, win);
	XCloseDisplay(dis);
	//exit(0);
}
void poll_event()


{
	if (XCheckWindowEvent(dis, win, event_mask, &event))
	{
		if (event.type == Expose)
		{
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
				//close_x11();
			}
		}
	}
}
void redraw()

{
	XWindowAttributes attr;
	XGetWindowAttributes(dis, win, &attr);

	if (!buff_ximage)
	{
		buff_ximage = XCreateImage(dis, DefaultVisual(dis, screen), 24, ZPixmap, 0, (char *)buff, WIDTH, HEIGHT, 32, 0);
		XInitImage(buff_ximage);
	}

	XPutImage(dis, win, gc, buff_ximage, 0, 0, 0, 0, WIDTH, HEIGHT);
	XMapWindow(dis, win);
	//XClearWindow(dis, win);
}
XImage set_ximage()
{
	XImage xi;
	xi.width = WIDTH;
	xi.height = HEIGHT;
	xi.xoffset = 0;			/* number of pixels offset in X direction */
	xi.format = ZPixmap;			/* XYBitmap, XYPixmap, ZPixmap */
	xi.data = (char *) buff;			/* pointer to image data */
	xi.byte_order = LSBFirst;			/* data byte order, LSBFirst, MSBFirst */
	xi.bitmap_unit = 32;		/* quant. of scanline 8, 16, 32 */
	xi.bitmap_bit_order = MSBFirst;		/* LSBFirst, MSBFirst */
	xi.bitmap_pad = 32;			/* 8, 16, 32 either XY or ZPixmap */
	xi.depth = 32;			/* depth of image */
	xi.bytes_per_line = 4 * WIDTH;		/* accelerator to next scanline */
	xi.bits_per_pixel = 32;		/* bits per pixel (ZPixmap) */
	return xi;
}
#endif
