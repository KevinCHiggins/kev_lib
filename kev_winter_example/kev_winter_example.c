#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_perf_timer.h"
#include "kev_render.h"

#define WIDTH 320
#define HEIGHT 240

void sleep_approx_ns(int64_t target_time_ns);

#define FRAME_RATE 60
#define FRAME_TIME_NS (1000000000 / FRAME_RATE)
kev_perf_timing timing;

char title[] = "kev_winter Example";


int64_t regulate_frame_time(int64_t target_time_ns)
{
	int64_t processing_time_ns = kev_perf_time_since_last_call_ns(&timing);
	int64_t leftover_ns = FRAME_TIME_NS - processing_time_ns;
	if (leftover_ns > 0)
	{
		sleep_approx_ns(leftover_ns);
	}
	int64_t sleeping_time_ns = kev_perf_time_since_last_call_ns(&timing);
	return processing_time_ns + sleeping_time_ns;
}

void spinning_diamond(kev_render_buffer buff, int offset, int size, unsigned int rgb)
{
	float smaller = 0.78539816339 + offset / 100.0;
	int orig_x = 100;
	int orig_y = 30;
	int s = sin(smaller) * size;
	int c = cos(smaller) * size;
	//kev_render_line(buff, orig_x + sin(smaller) * 100, orig_y + cos(smaller) * 100, orig_x + cos(smaller) * 100, orig_y - cos(smaller) * 100);

	kev_render_line(buff, orig_x + s, orig_y + c, orig_x + c, orig_y - s, rgb);
	kev_render_line(buff, orig_x + c, orig_y - s, orig_x - s, orig_y - c, rgb);
	kev_render_line(buff, orig_x - s, orig_y - c, orig_x - c, orig_y + s, rgb);
	kev_render_line(buff, orig_x - c, orig_y + s, orig_x + s, orig_y + c, rgb);
}
int run()
{
	uint32_t buff[WIDTH * HEIGHT];
	memset(&buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));

	kev_win win = {
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


	kev_perf_init();
	kev_win_init(&win);
	memset(&timing, 0, sizeof(kev_perf_timing));
	int off = 0;
	int64_t frame_time;
	unsigned int white = kev_render_rgb(255, 255, 255);
	while (1)
	{
		//kev_render_vert_line(render_buffer, 20, -1,19, white);
		kev_render_test_pattern(render_buffer, off);
		off++;
		/*
		kev_render_horiz_line(render_buffer, 100, 120, 0, white);
		kev_render_test_pattern(render_buffer, off);
		kev_render_horiz_line(render_buffer, 12, 24, 30, white);
		kev_render_vert_line(render_buffer, 20, -1,19, white);
		kev_render_horiz_line(render_buffer, 315, 320, 18, white);
		kev_render_horiz_line(render_buffer, 315, 319, 20, white);
		kev_render_horiz_line(render_buffer, 315, 318, 22, white);
		kev_render_vert_line(render_buffer, 18, 180, 240, white);

		kev_render_vert_line(render_buffer, 20, 180, 239, white);

		kev_render_vert_line(render_buffer, 22, 180, 238, white);

		kev_render_digit(render_buffer, 100, 100, 20, 40, '0', white);
		kev_render_digit(render_buffer, 120, 100, 20, 40, '1', white);
		kev_render_digit(render_buffer, 140, 100, 20, 40, '2', white);
		kev_render_digit(render_buffer, 160, 100, 20, 40, '3', white);
		kev_render_digit(render_buffer, 180, 100, 20, 40, '4', white);
		kev_render_digit(render_buffer, 200, 100, 20, 40, '5', white);
		kev_render_digit(render_buffer, 220, 100, 20, 40, '6', white);
		kev_render_digit(render_buffer, 240, 100, 20, 40, '7', white);
		kev_render_digit(render_buffer, 260, 100, 20, 40, '8', white);
		kev_render_digit(render_buffer, 280, 100, 20, 40, '9', white);
		kev_render_int(render_buffer, 120, 140, 20, 40, 1234567890, white);
		kev_render_int(render_buffer, 120, 180, 20, 40, (int)(1.0 / (frame_time / 1000000000.0)), white);
		kev_render_line(render_buffer, 210, 190, 230, 195, white);
		kev_render_line(render_buffer, 230, 195, 250, 190, white);
		kev_render_line(render_buffer, 40, 300, 50, 200, white);
		kev_render_line(render_buffer, 50, 200, 60, 300, white);
		*/
		spinning_diamond(render_buffer, off, 100, white);
		kev_win_poll_event(&win);
		frame_time = regulate_frame_time(FRAME_TIME_NS);

	}
}

#ifdef _WIN32
#include <windows.h>

void sleep_approx_ns(int64_t ns)
{
	Sleep(ns / 1000000);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR args_str, int n_cmd_show)
{
	//FILE* fp;

	//AllocConsole();
	//freopen_s(&fp, "CONIN$", "r", stdin);
	//freopen_s(&fp, "CONOUT$", "w", stdout);
	//freopen_s(&fp, "CONOUT$", "w", stderr);
	return run();


	//MessageBox(NULL, "Loop finished", "Error", MB_ICONEXCLAMATION | MB_OK);

}


#endif

#ifdef __linux__

void sleep_approx_ns(int64_t ns)
{
	struct timespec requested, remaining;
	requested.tv_sec = 0;
	requested.tv_nsec = ns;
	nanosleep(&requested, &remaining); // not currently handling errors and retries
}


int main()
{
	return run();
}
#endif
