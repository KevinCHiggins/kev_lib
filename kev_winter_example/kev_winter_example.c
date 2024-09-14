#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_perf_timer.h"

#define WIDTH 320
#define HEIGHT 240

void sleep_approx_ns(int64_t target_time_ns);

#define FRAME_RATE 60
#define FRAME_TIME_NS (1000000000 / FRAME_RATE)
kev_perf_timing timing;

char title[] = "kev_winter Example";

unsigned int rgb(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int result = 0;
	result += r << 16;
	result += g << 8;
	result += b;
	return result;
}

void fill_buffer(uint32_t *buff, int offset)
{
	for (int scanline = 0; scanline < HEIGHT; scanline++)
	{
		for (int pixel = 0; pixel < WIDTH; pixel += 1)
		{
			buff[scanline * WIDTH + pixel] = rgb(scanline, pixel + offset, scanline * (pixel + offset));
		}
	}
}


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

int run()

{
	uint32_t buff[WIDTH * HEIGHT];

	kev_win win = {
	.width = WIDTH,
	.height = HEIGHT,
	.title = title,
	.buffer = buff
	};


	kev_perf_init();
	kev_win_init(&win);
	memset(&timing, 0, sizeof(kev_perf_timing));
	int off = 0;

	while (1)
	{
		off++;
		fill_buffer(buff, off);
		kev_win_poll_event(&win);
		int64_t frame_time = regulate_frame_time(FRAME_TIME_NS);

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
	FILE* fp;

	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
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
