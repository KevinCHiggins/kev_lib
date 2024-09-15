#include <string.h>
#include <stdio.h>
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

void kev_render_int(kev_render_buffer buff, int x, int y, int digit_width, int digit_height, int number)
{
    char formatted[50];
    snprintf(formatted, 50, "%d", number);
    int i = 0;
    while (formatted[i] != 0)
    {
        kev_render_digit(buff, x + i * digit_width, y, 20, 40, formatted[i]);
        i++;
    }
}

void kev_render_digit(kev_render_buffer buff, int x, int y, int width, int height, char digit)
{
    // to avoid wasting pixels at bottom and right we position lines sub-pixel (float arithmetic) then round
    int left = round(width / 4.0);
    int right = round(width * 3.0 / 4.0);
    int top = round(height / 6.0);
    int mid = round(height / 2.0);
    int bottom = round(height * 5.0 / 6.0);
    switch (digit)
    {
    case '0':
        kev_render_horiz_line(buff, x + left, x + right, y + top);
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom);
        kev_render_vert_line(buff, x + left, y + bottom, y + top);
        break;

    case '1':
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        break;
    case '2':
        kev_render_horiz_line(buff, x + left, x + right, y + top);
        kev_render_vert_line(buff, x + right, y + top, y + mid);
        kev_render_horiz_line(buff, x + right, x + left, y + mid);
        kev_render_vert_line(buff, x + left, y + mid, y + bottom);
        kev_render_horiz_line(buff, x + left, x + right, y + bottom);
        break;
    case '3':
        kev_render_horiz_line(buff, x + left, x + right, y + top);
        kev_render_vert_line(buff, x + right, y + top, y + mid);
        kev_render_horiz_line(buff, x + right, x + left, y + mid);
        kev_render_vert_line(buff, x + right, y + mid, y + bottom);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom);
        break;
    case '4':
        kev_render_vert_line(buff, x + left, y + top, y + mid);
        kev_render_horiz_line(buff, x + left, x + right, y + mid);
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        break;
    case '5':
        kev_render_horiz_line(buff, x + right, x + left, y + top);
        kev_render_vert_line(buff, x + left, y + top, y + mid);
        kev_render_horiz_line(buff, x + left, x + right, y + mid);
        kev_render_vert_line(buff, x + right, y + mid, y + bottom);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom);
        break;
    case '6':
        kev_render_horiz_line(buff, x + right, x + left, y + top);
        kev_render_vert_line(buff, x + left, y + top, y + bottom);
        kev_render_horiz_line(buff, x + left, x + right, y + bottom);
        kev_render_vert_line(buff, x + right, y + bottom, y + mid);
        kev_render_horiz_line(buff, x + right, x + left, y + mid);
        break;
    case '7':
        kev_render_horiz_line(buff, x + left, x + right, y + top);
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        break;
    case '8':
        kev_render_horiz_line(buff, x + left, x + right, y + top);
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom);
        kev_render_vert_line(buff, x + left, y + bottom, y + top);
        kev_render_horiz_line(buff, x + left, x + right, y + mid);
        break;
    case '9':
        kev_render_horiz_line(buff, x + right, x + left, y + top);
        kev_render_vert_line(buff, x + left, y + top, y + mid);
        kev_render_horiz_line(buff, x + left, x + right, y + mid);
        kev_render_vert_line(buff, x + right, y + top, y + bottom);
        break;
    }
}