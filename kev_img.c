#include "kev_img.h"

char *read_tga_pixel_data(char *filename)
{
	FILE *fptr = fopen(filename, "r");
	printf("Size of struct %ld\n", sizeof(kev_img_tga_header));
	if (fptr == NULL) exit(1);
	kev_img_tga_header header;
	fread(&header, 18, 1, fptr);
	printf("Descriptor length %d\nColormap type %d\nType %d\nColormap origin %d\nColormap_length %d\nColormap depth %d\nX %d\nY %d\nW %d\nH %d\nBpp %d\n", header.image_descriptor_length, header.colormap_type, header.type, header.colormap_origin, header.colormap_length, header.colormap_depth, header.x_origin, header.y_origin, header.width, header.height, header.bpp);
	if (header.type != 2)
	{
		printf("Only type 2 TGA images (uncompressed RGB) accepted");
		exit(1);
	}
	char *pixels; // consider making a pixel struct...
	size_t bytes_per_pixel = header.bpp / 8;
	pixels = (char *)calloc(header.width * header.height, bytes_per_pixel);
	fseek(fptr, header.image_descriptor_length, SEEK_CUR);
	fread(pixels, header.width * header.height * bytes_per_pixel, 1, fptr);
	return pixels;
}