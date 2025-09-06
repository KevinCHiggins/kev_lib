#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"
#include "kev_perf_timer.h"
#include "kev_render.h"
#include "kev_test.h"

#define WIDTH 320
#define HEIGHT 240
#define SCALE 4

void sleep_approx_ns(int64_t target_time_ns);

#define FRAME_RATE 60
#define FRAME_TIME_NS (1000000000 / FRAME_RATE)
kev_perf_timing timing;

uint32_t buff[WIDTH * HEIGHT];
double ang = 0.0;
char title[] = "kev_render Lines Animation Test";
kev_win win;
kev_render_buffer render_buffer;
unsigned int white;
unsigned int reddish;
unsigned int blue;
unsigned int green;
void setup()
{
    white = kev_render_rgb(255, 255, 255);
    reddish = kev_render_rgb(100, 15, 35);
    blue = kev_render_rgb(10, 50, 255);
    green = kev_render_rgb(10, 255, 39);
    render_buffer = (kev_render_buffer) {
        .width = WIDTH,
        .height = HEIGHT,
        .bpp = 32,
        .buffer = buff
    };
}
int check_point(int x, int y)
{
    return (buff[y * WIDTH + x] == white);
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

void test_float_line()
{

    kev_render_float_line(render_buffer, 220.4, 200.4, 238.6, 209.6, white);
    kev_render_float_line(render_buffer, 220.4, 200.4, 239.6, 189.6, green);
    // kev_render_float_line(render_buffer, 0.0, 211.0, 239.0, 211.0, blue);
    
}


int run()
{

    

    win = (kev_win){
    .width = WIDTH,
    .height = HEIGHT,
    .scale = SCALE,
    .title = title,
    .buffer = buff
    };

    kev_perf_init();
    kev_win_init(&win);
    memset(&timing, 0, sizeof(kev_perf_timing));
    int64_t frame_time;
    setup();
    
    while (1)
    {

        kev_render_fill(render_buffer, reddish);

        kev_render_float_line(render_buffer, 160.0, 120.0, 170.0 + ang, 170.0, white);
        kev_win_update_events(&win);
        frame_time = regulate_frame_time(FRAME_TIME_NS);
        ang += 0.01;
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

