#define _POSIX_C_SOURCE 199309L
#include <stdio.h>

#include <stdint.h>
#include <stdlib.h>
#include "kev_render.h"
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


kev_render_buffer kev_img_from_tga(char * filename);
