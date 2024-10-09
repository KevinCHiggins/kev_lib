#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_perf_timer.h"
#include "kev_render.h"

#define WIDTH 640
#define HEIGHT 480
#define SCALE 2

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
float slice_texture_offset[WIDTH];
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
float player_acc = 0.01;
float player_vel = 0.000;
float player_max_vel = 0.005;
double player_rads = 5.0;
double two_pi = M_PI * 2;
double rads_to_degrees(double rads)
{
	return rads / M_PI * 180.0;
}
double degrees_to_rads(double degrees)
{
	return degrees / 180.0 * M_PI;
}

double fov_rads;

int is_wall(int x, int y)
{
	return walls[y][x];
}

void update_player()
{
	if (kev_win_is_pressed(KEYCODE_RIGHT))
	{
		printf("Right\n");
		player_rads -= 0.008;
	}
	if (kev_win_is_pressed(KEYCODE_LEFT))
	{
		printf("Left\n");
		player_rads += 0.008;
	}
	if (kev_win_is_pressed(KEYCODE_UP) && player_vel < player_max_vel)
	{
		printf("Up\n");
		player_vel += player_acc;
	}
	if (kev_win_is_pressed(KEYCODE_DOWN) && player_vel > (0 - player_max_vel))
	{
		printf("Down\n");
		player_vel -= player_acc;
	}
	player_vel = player_vel * 0.75;

	while (player_rads >= two_pi)
	{
		player_rads -= two_pi;
	}
	float dx = cos(player_rads) * player_vel;
	float dy = (0 - sin(player_rads)) * player_vel;
	if (is_wall(floor(player_x), floor(player_y + dy)))
	{
		dy = 0;
	}
	if (is_wall(floor(player_x + dx), floor(player_y)))
	{
		dx = 0;
	}
	player_x += dx;
	player_y += dy;
}



double dist_to_wall(double ang, double player_x, double player_y, float *across)
{
	int player_x_floor = (int)floor(player_x);
	int player_y_floor = (int)floor(player_y);
	double ns_crossing_dist = fabs(1 / cos(ang));
	double we_crossing_dist = fabs(1 / sin(ang));
	double ray_dist_to_ns, ray_dist_to_we;
	int x_crossed = 0;
	int y_crossed = 0;
	int x_inc;
	int y_inc;
	if (ang < M_PI / 2)
	{
		ray_dist_to_ns = ns_crossing_dist * (1 - (fmod(player_x, 1.0)));
		ray_dist_to_we = we_crossing_dist * (fmod(player_y, 1.0));
		x_inc = 1;
		y_inc = -1;
	}
	else if (ang < M_PI)
	{
		ray_dist_to_ns = ns_crossing_dist * (fmod(player_x, 1.0));
		ray_dist_to_we = we_crossing_dist * (fmod(player_y, 1.0));
		x_inc = -1;
		y_inc = -1;
	}
	else if (ang < M_PI * 1.5)
	{
		ray_dist_to_ns = ns_crossing_dist * (fmod(player_x, 1.0));
		ray_dist_to_we = we_crossing_dist * (1 - (fmod(player_y, 1.0)));
		x_inc = -1;
		y_inc = 1;
	}
	else
	{
		ray_dist_to_ns = ns_crossing_dist * (1 - (fmod(player_x, 1.0)));
		ray_dist_to_we = we_crossing_dist * (1 - (fmod(player_y, 1.0)));
		x_inc = 1;
		y_inc = 1;
	}

	while (1)
	{
		if (ray_dist_to_ns > ray_dist_to_we)
		{
			y_crossed += y_inc;
			if (is_wall(player_x_floor + x_crossed, player_y_floor + y_crossed))
				{
					*across = ray_dist_to_we;
					return ray_dist_to_we;
				}
			ray_dist_to_we += we_crossing_dist;
		}
		else
		{
			x_crossed += x_inc;
			if (is_wall(player_x_floor + x_crossed, player_y_floor + y_crossed))
			{
				*across = ray_dist_to_ns;
				return ray_dist_to_ns;
			}
			ray_dist_to_ns += ns_crossing_dist;
		}
	}
}

void fill_slice_heights()
{
	for (int i = 0; i < WIDTH; i++)
	{
		float across = 0;
		double dist = dist_to_wall(player_rads + ray_rads[i], player_x, player_y, &across);
		slice_heights[i] = (HEIGHT / 2) / (dist * cos(0 - ray_rads[i]));
		slice_texture_offset[i] = across;
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
	fov_rads = degrees_to_rads(60.0);
	double step_rads = fov_rads / WIDTH;
	double ang_rads = fov_rads / 2;
	for (int i = 0; i < WIDTH; i++)
	{
		ray_rads[i] = ang_rads;
		ang_rads -= step_rads;
	}
	printf("%f", ray_rads[319]);
}

void rotate_point(float *x, float *y, float ang)
{
	float c = cos(ang);
	float s = sin(ang);
	float temp;
	temp = c * *x - s * *y;
	*y = s **x + c * *y;
	*x = temp;
}
void draw_line_relative_to_player(kev_render_buffer buff, float x1, float y1, float x2, float y2, unsigned int rgb)
{
	x1 = x1 - player_x;
	y1 = y1 - player_y;
	x2 = x2 - player_x;
	y2 = y2 - player_y;
	rotate_point(&x1, &y1, player_rads - M_PI / 2);
	rotate_point(&x2, &y2, player_rads - M_PI / 2);
	kev_render_line(buff, (x1 * 40) + (WIDTH / 2), (y1 * 40) + (HEIGHT / 2), (x2 * 40) + (WIDTH / 2), (y2 * 40) + (HEIGHT / 2), rgb);
}

int run()
{
	uint32_t buff[WIDTH * HEIGHT];
	memset(&buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));

	kev_win win = {
	.width = WIDTH,
	.height = HEIGHT,
	.scale = SCALE,
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
	unsigned int blueish = kev_render_rgb(30, 70, 150);
	unsigned int reddish = kev_render_rgb(160, 50, 10);
	unsigned int white = kev_render_rgb(199, 199, 199);
	while (1)
	{
		memset(&buff, 0, WIDTH * HEIGHT * sizeof(uint32_t));
		fill_slice_heights();
		int horizon_y = HEIGHT / 2;

		for (int i = 0; i < WIDTH; i++)
		{
			unsigned int rgb = kev_render_rgb(10, 20, (int)(slice_texture_offset[i] * 20));
			kev_render_vert_line(render_buffer, i, horizon_y - slice_heights[i], horizon_y + slice_heights[i], rgb);
		}
		

		int grid_size_x = HEIGHT / ARENA_HEIGHT;
		int grid_size_y = HEIGHT / ARENA_HEIGHT;
		for (int arena_y = 0; arena_y < ARENA_HEIGHT; arena_y++)
		{
			for (int arena_x = 0; arena_x < ARENA_WIDTH; arena_x++)
			{
				if (walls[arena_y][arena_x])
				{
					if (arena_y == 0 || !walls[arena_y - 1][arena_x])
					{
						draw_line_relative_to_player(render_buffer, arena_x, arena_y, (arena_x + 1), arena_y, white);
					}
					if (arena_x == 0 || !walls[arena_y][arena_x - 1])
					{
						draw_line_relative_to_player(render_buffer, arena_x, arena_y, arena_x, (arena_y + 1), white);
					}
					if (arena_y == (ARENA_HEIGHT - 1) || !walls[arena_y + 1][arena_x])
					{
						draw_line_relative_to_player(render_buffer, arena_x, (arena_y + 1), (arena_x + 1), (arena_y + 1), white);
					}
					if (arena_x == (ARENA_WIDTH - 1) || !walls[arena_y][arena_x + 1])
					{
						draw_line_relative_to_player(render_buffer, (arena_x + 1), arena_y, (arena_x + 1), (arena_y + 1), white);
					}
					//kev_render_rectangle(render_buffer, arena_x * grid_size_x, arena_y * grid_size_y, (arena_x + 1) * grid_size_x, (arena_y + 1) * grid_size_y, white);
				}
			}
		}
		double ang_to_left = player_rads + ray_rads[0];
		double ang_to_right = player_rads + ray_rads[WIDTH - 1];
		double left_x = player_x + cos(ang_to_left);
		double left_y = player_y + 0 - sin(ang_to_left);
		double right_x = player_x + cos(ang_to_right);
		double right_y = player_y + 0 - sin(ang_to_right);

		kev_render_line(render_buffer, WIDTH / 2, HEIGHT / 2, WIDTH / 2 + 30, HEIGHT / 2 - 30 * 1.732, white);
		kev_render_line(render_buffer, WIDTH / 2, HEIGHT / 2, WIDTH / 2 - 30, HEIGHT / 2 - 30 * 1.732, white);
		//kev_render_line(render_buffer, player_x * grid_size_x, player_y * grid_size_y, right_x * grid_size_x, right_y * grid_size_y, white);
		//kev_render_line(render_buffer, left_x * grid_size_x, left_y * grid_size_y, right_x * grid_size_x, right_y * grid_size_y, white);
		kev_win_update_events(&win);
		
		update_player();
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
	FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
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
