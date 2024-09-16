#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_perf_timer.h"
#include "kev_render.h"

#define WIDTH 320
#define HEIGHT 240

#define ARENA_WIDTH 8
#define ARENA_HEIGHT 8

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif


void sleep_approx_ns(int64_t target_time_ns);

#define FRAME_RATE 60
#define FRAME_TIME_NS (1000000000 / FRAME_RATE)
kev_perf_timing timing;

char title[] = "kev_caster_2";

int slice_heights[WIDTH];
double ray_rads[WIDTH];
int walls[ARENA_WIDTH][ARENA_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 0, 1, 1, 1},
	{1, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};
float player_x = 4.5;
float player_y = 4.5;
double player_rads = 0.0;
double two_pi = M_PI * 2;
double rads_to_degrees(double rads)
{
	return rads / M_PI * 180.0;
}
double degrees_to_rads(double degrees)
{
	return degrees / 180.0 * M_PI;
}

double fov;

void update_player()
{
	player_rads += 0.001;
	while (player_rads >= two_pi)
	{
		player_rads -= two_pi;
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

void init()
{
	fov = degrees_to_rads(60.0);
	double fov_left_tan = tan(fov / 2);
	double step = (fov_left_tan * 2) / WIDTH;
	for (int i = 0; i < WIDTH; i++)
	{
		slice_heights[i] = i;
		ray_rads[i] = atan(fov_left_tan);
		fov_left_tan -= step;
	}
	printf("%f", ray_rads[319]);
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

	init();
	kev_perf_init();
	kev_win_init(&win);
	memset(&timing, 0, sizeof(kev_perf_timing));
	int off = 0;
	int64_t frame_time;
	while (1)
	{
		/*
		int horizon_y = HEIGHT / 2;
		for (int i = 0; i < WIDTH; i++)
		{
			kev_render_vert_line(render_buffer, i, horizon_y - slice_heights[i], horizon_y + slice_heights[i]);
		}
		*/
		update_player();
		memset(&buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));
		int grid_size_x = HEIGHT / ARENA_HEIGHT;
		int grid_size_y = HEIGHT / ARENA_HEIGHT;
		for (int arena_y = 0; arena_y < ARENA_HEIGHT; arena_y++)
		{
			for (int arena_x = 0; arena_x < ARENA_WIDTH; arena_x++)
			{
				if (walls[arena_y][arena_x])
				{
					kev_render_rectangle(render_buffer, arena_x * grid_size_x, arena_y * grid_size_y, (arena_x + 1) * grid_size_x, (arena_y + 1) * grid_size_y);
				}
			}
		}
		double ang_to_left = player_rads + ray_rads[0];
		double ang_to_right = player_rads + ray_rads[WIDTH - 1];
		double left_x = player_x + cos(ang_to_left);
		double left_y = player_y + 0 - sin(ang_to_left);
		double right_x = player_x + cos(ang_to_right);
		double right_y = player_y + 0 - sin(ang_to_right);

		kev_render_line(render_buffer, player_x * grid_size_x, player_y * grid_size_y, left_x * grid_size_x, left_y * grid_size_y);
		kev_render_line(render_buffer, player_x * grid_size_x, player_y * grid_size_y, right_x * grid_size_x, right_y * grid_size_y);
		kev_render_line(render_buffer, left_x * grid_size_x, left_y * grid_size_y, right_x * grid_size_x, right_y * grid_size_y);
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

	return run();

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
