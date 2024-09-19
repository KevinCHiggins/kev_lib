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
kev_render_buffer render_buffer;
unsigned int white;
unsigned int reddish;

void setup()
{
	white = kev_render_rgb(255, 255, 255);
	reddish = kev_render_rgb(160, 5, 75);
	render_buffer = (kev_render_buffer) {
		.width = WIDTH,
		.height = HEIGHT,
		.bpp = 32,
		.buffer = buff
	};
	kev_render_fill(render_buffer, reddish);
}
int check_point(int x, int y)
{
	return (buff[y * WIDTH + x] == white);
}
void test_points()
{

	int x = 20;
	int y = 20;
	kev_render_point(render_buffer, x, y, white);
	kev_test_assert_true(buff[y * WIDTH + x] == white);
}
void test_horiz_line_pos_x()
{
	int x1 = 255;
	int y1 = 5;
	int x2 = 265;
	int y2 = 5;
	kev_render_line(render_buffer, x1, y1, x2, y1, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y1));
	kev_test_assert_false(check_point(x1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1));
	kev_test_assert_false(check_point(x1 - 1, y1 + 1));
	kev_test_assert_false(check_point(x1, y1 + 1));

	kev_test_assert_false(check_point(x2, y2 - 1));
	kev_test_assert_false(check_point(x2 + 1, y2 - 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
	kev_test_assert_false(check_point(x1 + 1, y2 + 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
}
void test_horiz_line_neg_x()
{
	int x1 = 305;
	int y1 = 5;
	int x2 = 315;
	int y2 = 5;
	kev_render_line(render_buffer, x2, y1, x1, y1, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y1));
	kev_test_assert_false(check_point(x1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1));
	kev_test_assert_false(check_point(x1 - 1, y1 + 1));
	kev_test_assert_false(check_point(x1, y1 + 1));

	kev_test_assert_false(check_point(x2, y2 - 1));
	kev_test_assert_false(check_point(x2 + 1, y2 - 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
	kev_test_assert_false(check_point(x1 + 1, y2 + 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
}
void test_low_slope_line_pos_x_pos_y()
{
	int x1 = 55;
	int y1 = 5;
	int x2 = 65;
	int y2 = 10;
	kev_render_line(render_buffer, x1, y1, x2, y2, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y2)); // second attempt requires extra point drawn
	kev_test_assert_false(check_point(x1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1));
	kev_test_assert_false(check_point(x1 - 1, y1 - 1));
	kev_test_assert_false(check_point(x2, y2 + 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
	kev_test_assert_false(check_point(x2 + 1, y2 + 1)); // first attempt at algorithm fails here
}
void test_low_slope_line_neg_x_neg_y()
{
	int x1 = 115;
	int y1 = 10;
	int x2 = 105;
	int y2 = 5;

	kev_render_line(render_buffer, x1, y1, x2, y2, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y2));
	kev_test_assert_false(check_point(x1, y1 + 1));
	kev_test_assert_false(check_point(x1 + 1, y1));
	kev_test_assert_false(check_point(x1 + 1, y1 + 1));
	kev_test_assert_false(check_point(x2, y2 - 1));
	kev_test_assert_false(check_point(x2 - 1, y2));
	kev_test_assert_false(check_point(x2 - 1, y2 - 1)); 
}
void test_low_slope_line_neg_x_pos_y()
{
	int x1 = 155;
	int y1 = 5;
	int x2 = 145;
	int y2 = 10;
	kev_render_line(render_buffer, x1, y1, x2, y2, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y2)); 
	kev_test_assert_false(check_point(x1, y1 - 1));
	kev_test_assert_false(check_point(x1 + 1, y1));
	kev_test_assert_false(check_point(x1 + 1, y1 - 1));
	kev_test_assert_false(check_point(x2, y2 + 1));
	kev_test_assert_false(check_point(x2 - 1, y2));
	kev_test_assert_false(check_point(x2 - 1, y2 + 1));
}
void test_low_slope_line_pos_x_neg_y()
{
	int x1 = 205;
	int y1 = 10;
	int x2 = 215;
	int y2 = 5;

	kev_render_line(render_buffer, x1, y1, x2, y2, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y2));
	kev_test_assert_false(check_point(x1, y1 + 1));
	kev_test_assert_false(check_point(x1 - 1, y1));
	kev_test_assert_false(check_point(x1 - 1, y1 + 1));
	kev_test_assert_false(check_point(x2, y2 - 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
	kev_test_assert_false(check_point(x2 + 1, y2 - 1)); 
}
void test_high_slope_line_pos_x_pos_y()
{
	int x1 = 55;
	int y1 = 50;
	int x2 = 60;
	int y2 = 60;
	kev_render_line(render_buffer, x1, y1, x2, y2, white);
	kev_test_assert_true(check_point(x1, y1));
	kev_test_assert_true(check_point(x2, y2)); // second attempt requires extra point drawn
	kev_test_assert_false(check_point(x1, y1 - 1));
	kev_test_assert_false(check_point(x1 - 1, y1));
	kev_test_assert_false(check_point(x1 - 1, y1 - 1));
	kev_test_assert_false(check_point(x2, y2 + 1));
	kev_test_assert_false(check_point(x2 + 1, y2));
	kev_test_assert_false(check_point(x2 + 1, y2 + 1)); // first attempt at algorithm fails here
}
int display()
{

	

	win = (kev_win){
	.width = WIDTH,
	.height = HEIGHT,
	.title = title,
	.buffer = buff
	};




	kev_win_init(&win);
	
	while (1)
	{

		kev_win_poll_event(&win);

	}
}

void run_all_tests()
{
	kev_test_run("Drawing points.", test_points);
	kev_test_run("Low slope line, positive x, positive y", test_low_slope_line_pos_x_pos_y);
	kev_test_run("Low slope line, negative x, negative y", test_low_slope_line_neg_x_neg_y);
	kev_test_run("Low slope line, positive x, negative y", test_low_slope_line_pos_x_neg_y);
	kev_test_run("Low slope line, negative x, positive y", test_low_slope_line_neg_x_pos_y);
	kev_test_run("Horiz line, positive x", test_horiz_line_pos_x);
	kev_test_run("Horiz line, negative x", test_horiz_line_neg_x);
	kev_test_run("High slope line, positive x, positive y", test_high_slope_line_pos_x_pos_y);
	puts(kev_test_get_report());
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

