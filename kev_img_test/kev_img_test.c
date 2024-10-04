#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_render.h"
#include "kev_img.h"
#include "kev_test.h"

#define WIDTH 320
#define HEIGHT 240
#define SCALE 4

uint32_t buff[WIDTH * HEIGHT];
char title[] = "kev_render Test";
kev_win win;
kev_render_buffer render_buffer;
kev_img img;
unsigned int white;
unsigned int reddish;
unsigned int cyanish;

void setup()
{
	white = kev_render_rgb(255, 255, 255);
	cyanish = kev_render_rgb(31, 249, 251);
	reddish = kev_render_rgb(160, 5, 75);
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
	char rgb_string[11];
	printf("Found");
	kev_render_debug_rgb(buff[y * WIDTH + x]);
	printf("Seeking ");
	kev_render_debug_rgb(rgb);
	return (buff[y * WIDTH + x] == rgb);
}
void test_points()
{

	int x = 20;
	int y = 20;
	img = from_tga("32x32.tga");
	kev_render_img(render_buffer, x, y, 32, 32, img.pixels);
	check_point(x, y, kev_render_rgb(31, 249, 251));
	check_point(x + 1, y, kev_render_rgb(1, 1, 1));
	check_point(x + 2, y, kev_render_rgb(129, 106, 162));
	check_point(x + 3, y, kev_render_rgb(129, 104, 162));
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
	kev_test_run("Drawing points.", test_points);
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR args_str, int n_cmd_show)
{
	setup();
	run_all_tests();
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

