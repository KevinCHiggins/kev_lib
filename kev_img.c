#include "kev_img.h"

kev_img kev_img_from_tga(char *filename)
{
	FILE *fptr = fopen(filename, "r");
	printf("Size of struct %ld\n", sizeof(kev_img_tga_header));
	if (fptr == NULL)
		{
			printf("Failed to open file\n");
			exit(1);
		}
	kev_img_tga_header header;
	int num_objects_read = fread(&header, 18, 1, fptr);
	if (num_objects_read != 18)
	{
		printf("Failed to read TGA image header\n");
	}
	printf("Descriptor length %d\nColormap type %d\nType %d\nColormap origin %d\nColormap_length %d\nColormap depth %d\nX %d\nY %d\nW %d\nH %d\nBpp %d\n", header.image_descriptor_length, header.colormap_type, header.type, header.colormap_origin, header.colormap_length, header.colormap_depth, header.x_origin, header.y_origin, header.width, header.height, header.bpp);
	if (header.type != 2)
	{
		printf("Only type 2 TGA images (uncompressed RGB) accepted\n");
		exit(1);
	}
	unsigned char *raw_pixels; // because signed types have undefined behavior when left shifted
	uint32_t *pixels;
	int num_pixels = header.width * header.height;
	size_t bytes_per_pixel = header.bpp / 8;
	pixels = (uint32_t *)calloc(num_pixels, sizeof(uint32_t));
	raw_pixels = (unsigned char *)calloc(num_pixels, bytes_per_pixel);


	fseek(fptr, header.image_descriptor_length, SEEK_CUR);
	num_objects_read = fread(raw_pixels, header.width * header.height * bytes_per_pixel, 1, fptr);
	if (num_objects_read != header.width * header.height * bytes_per_pixel)
	{
		printf("Failed to read TGA image data\n");
	}
	if (bytes_per_pixel == 3)
	{
		for (int pixel = 0; pixel < num_pixels; pixel++)
		{
			uint32_t val = (raw_pixels[pixel * bytes_per_pixel]) + (raw_pixels[pixel * bytes_per_pixel + 1] << 8) + (raw_pixels[pixel * bytes_per_pixel + 2] << 16);
			pixels[pixel] = val;
		}
	}
	else
	{
		printf("TGA pixel data packing other than 3-byte not implemented\n");
		exit(1);
	}
	return (kev_img){
		(size_t) header.width,
		(size_t) header.height,
		bytes_per_pixel,
		pixels
	};
}