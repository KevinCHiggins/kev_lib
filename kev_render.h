#ifndef KEV_RENDER_H
#define KEV_RENDER_H
#include <stdint.h>
#include <math.h>

typedef struct kev_render_buffer {
	int width;
	int height;
	int bpp;
	uint32_t *buffer;
} kev_render_buffer;

typedef struct kev_render_rect {
	int x;
	int y;
	int width;
	int height;
} kev_render_rect;

void kev_render_test_pattern(kev_render_buffer buff, int anim_offset);
void kev_render_horiz_line(kev_render_buffer buff, int x1, int x2, int y, unsigned int rgb);
void kev_render_vert_line(kev_render_buffer buff, int x, int y1, int y2, unsigned int rgb);

void kev_render_line(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb);
void kev_render_rectangle(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb);
void kev_render_img(kev_render_buffer buff, int x1, int y1, int width, int height, uint32_t *pixels);
void kev_render_int(kev_render_buffer buff, int x, int y, int digit_width, int digit_height, int number, unsigned int rgb);
unsigned int kev_render_rgb(unsigned char r, unsigned char g, unsigned char b);
void kev_render_debug_rgb(unsigned int rgb);
void kev_render_digit(kev_render_buffer buff, int x, int y, int width, int height, char digit, unsigned int rgb);
void kev_render_point(kev_render_buffer buff, int x, int y, unsigned int rgb);
void kev_render_fill(kev_render_buffer buff, unsigned int rgb);
kev_render_buffer load_tga(char *path);

#endif
