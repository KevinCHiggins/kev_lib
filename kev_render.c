#include <string.h>
#include "kev_render.h"

unsigned int rgb(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int result = 0;
    result += r << 16;
    result += g << 8;
    result += b;
    return result;
}

void kev_render_test_pattern(kev_render_buffer buff, int anim_offset)
{
    switch (buff.bpp)
    {
        case 32:
            for (int row = 0; row < buff.height; row++)
            {
                for (int pixel = 0; pixel < buff.width; pixel++)
                {
                    int row_start_in_buff = row * buff.width;
                    buff.buffer[row_start_in_buff + pixel] = rgb(row, pixel + anim_offset, row * (pixel + anim_offset));
                }
            }
            break;
    }

}

void kev_render_horiz_line(kev_render_buffer buff, int x1, int x2, int y)
{
    if (x1 > x2)
    {
        int temp = x1;
        x1 = x2;
        x2 = temp;
    }

    if (x2 < 0) return;
    if (x1 >= buff.width) return;
    if (y < 0) return;
    if (y >= buff.height) return;
    if (x1 < 0) x1 = 0;
    if (x2 >= buff.width) x2 = buff.width - 1;
    int row_start_in_buff = y * buff.width;
    for (int x = x1; x < x2; x++)
    {
        buff.buffer[row_start_in_buff + x] = rgb(255, 255, 255);
    }
}
void kev_render_vert_line(kev_render_buffer buff, int x, int y1, int y2)
{
    if (y1 > y2)
    {
        int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    if (y2 < 0) return;
    if (y1 >= buff.height) return;
    if (x < 0) return;
    if (x >= buff.width) return;
    if (y1 < 0) y1 = 0;
    if (y2 >= buff.height) y2 = buff.height - 1;
    for (int y = y1; y < y2; y++)
    {
        int row_start_in_buff = y * buff.width;
        buff.buffer[row_start_in_buff + x] = rgb(255, 255, 255);
    }
}