#ifndef KEV_RENDER_H
#define KEV_RENDER_H
#include <stdint.h>

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
void kev_render_horiz_line(kev_render_buffer buff, int x1, int x2, int y);
void kev_render_vert_line(kev_render_buffer buff, int x, int y1, int y2);
void kev_render_blit_32_on_24(kev_render_buffer buff32, kev_render_buffer buff24, kev_render_rect rect1, kev_render_rect rect2);
kev_render_buffer load_tga(char *path);

#endif
