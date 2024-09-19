#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_render.h"
#include "kev_test.h"

#define WIDTH 320
#define HEIGHT 240

uint32_t buff[WIDTH * HEIGHT];
char title[] = "kev_render Test";
kev_win win;
unsigned int white;
void test_points()
{
    int x = 20;
    int y = 20;
    //kev_render_point(win.buffer, x, y, white);
    kev_test_assert_true(buff[y * WIDTH + x] == white);
}
int run()
{

	memset(&buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));

	win = (kev_win){
	.width = WIDTH,
	.height = HEIGHT,
	.title = title,
	.buffer = buff
	};

	kev_render_buffer render_buffer = {
		.width = WIDTH,
		.height = HEIGHT,
		.bpp = 32,
		.buffer = buff
	};

	white = kev_render_rgb(255, 255, 255);
	kev_win_init(&win);
	
	while (1)
	{

		kev_win_poll_event(&win);

	}
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR args_str, int n_cmd_show)
{
	kev_test_run("Drawing points.", test_points);

	puts(kev_test_get_report());
	return run();

}


#endif

#ifdef __linux__


int main()
{
	kev_test_run("Drawing points.", test_points);

	puts(kev_test_get_report());
	return run();
}
#endif

