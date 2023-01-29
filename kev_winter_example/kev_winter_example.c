#include <stdio.h>
#include <stdlib.h>
#include "kev_winter.h"

#define WIDTH 320
#define HEIGHT 240

char title[] = "kev_winter Example";

unsigned int rgb(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int result = 0;
	result += r << 16;
	result += g << 8;
	result += b;
	return result;
}

void fill_buffer(uint32_t *buff)
{
	for (int scanline = 0; scanline < HEIGHT; scanline++)
	{
		for (int pixel = 0; pixel < WIDTH; pixel += 1)
		{
			buff[scanline * WIDTH + pixel] = rgb(scanline, pixel, scanline * pixel);
		}
	}
}

int run()

{
	uint32_t buff[WIDTH * HEIGHT];
	fill_buffer(buff);
	kev_win win = {
	.width = WIDTH,
	.height = HEIGHT,
	.title = title,
	.buffer = buff
	};



	init(&win);

	while (1)
	{
		poll_event(&win);

	}
}

#ifdef _WIN32

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


int main()
{
	return run();
}
#endif
