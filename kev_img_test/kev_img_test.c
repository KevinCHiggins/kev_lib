#include "kev_img.h"

int main(int argc, char **argv)
{
	char *pixels = read_tga_pixel_data("32x32.tga");
	printf("First pixel %x\n", pixels[0]);
	char c;
}