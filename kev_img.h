#include "stdio.h"
#include "stdlib.h"
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
char *read_tga_pixel_data(char * filename);