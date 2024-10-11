#define _POSIX_C_SOURCE 199309L
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#pragma pack(push, 1)
typedef struct
{
	char image_descriptor_length;
	char colormap_type;
	char type;
	short int colormap_origin;
	short int colormap_length;
	char colormap_depth;
	short int x_origin;
	short int y_origin;
	short width;
	short height;
	char bpp;
	char image_descriptor;
} kev_img_tga_header;

typedef struct
{
	size_t width;
	size_t height;
	size_t bytes_per_pixel;
	uint32_t *pixels;
} kev_img;
kev_img kev_img_from_tga(char * filename);
