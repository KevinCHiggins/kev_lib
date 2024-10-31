#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_render.h"
#include "kev_img.h"
#include "kev_test.h"

#define WIDTH 800
#define HEIGHT 600
#define SCALE 1

uint32_t buff[WIDTH * HEIGHT];
char title[] = "kev_render Test";
kev_win win;
kev_render_buffer render_buffer;
kev_render_buffer img;
unsigned int white;
unsigned int reddish;
unsigned int cyanish;

void setup()
{
	render_buffer = (kev_render_buffer) {
		.width = WIDTH,
		.height = HEIGHT,
		.bpp = 32,
		.buffer = buff
	};
	kev_render_fill(render_buffer, reddish);

}

int check_point(int x, int y, unsigned int rgb)
{
	kev_test_assert_true(buff[y * WIDTH + x] == rgb);
}

void test_img()
{
	img = kev_img_from_tga("32x32.tga");

	int x = 0;
	int y = 0;
	kev_render_img(render_buffer, x, y, 32, 32, img);
	check_point(x, y, kev_render_rgb(31, 249, 251));
	check_point(x + 1, y, kev_render_rgb(1, 1, 1));
	check_point(x + 2, y, kev_render_rgb(129, 106, 162));
	check_point(x + 3, y, kev_render_rgb(129, 104, 162));
	check_point(x + 31, y + 31, kev_render_rgb(65, 231, 238));
}

void test_img_load_0x1a()
{
	img = kev_img_from_tga("test_0x1a.tga");

	int x = 32;
	int y = 32;
	kev_render_img(render_buffer, x, y, 1, 1, img);
	check_point(x, y, kev_render_rgb(26, 26, 26));
}

void test_img_load_larger()
{
	img = kev_img_from_tga("wall.tga");

	int x = 64;
	int y = 0;
	kev_render_img(render_buffer, x, y, 512, 512, img);
	check_point(x, y, kev_render_rgb(124, 120, 121));
}

int display()
{
	win = (kev_win){
	.width = WIDTH,
	.height = HEIGHT,
	.scale = SCALE,
	.title = title,
	.buffer = buff
	};

	kev_win_init(&win);
	
	while (1)
	{
		kev_win_update_events(&win);

	}
}

void run_all_tests()
{
	kev_test_run("Drawing image and checking some pixels.", test_img);
	kev_test_run("Regression of 0x1a being interpreted as EOF in Windows", test_img_load_0x1a);
	kev_test_run("Load larger image", test_img_load_larger);
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR args_str, int n_cmd_show)
{
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	setup();
	run_all_tests();
	puts(kev_test_get_report());
	return display();
}


#endif

#ifdef __linux__


int main()
{
	setup();
	run_all_tests();

	puts(kev_test_get_report());
	return display();
}

#endif
