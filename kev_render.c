#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include "kev_render.h"

unsigned int kev_render_rgb(unsigned char r, unsigned char g, unsigned char b)
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
                    buff.buffer[row_start_in_buff + pixel] = kev_render_rgb(row, pixel + anim_offset, row * (pixel + anim_offset));
                }
            }
            break;
    }

}

void kev_render_horiz_line(kev_render_buffer buff, int x1, int x2, int y, unsigned int rgb)
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
    for (int x = x1; x <= x2; x++)
    {
        buff.buffer[row_start_in_buff + x] = rgb;
    }
}
void kev_render_vert_line(kev_render_buffer buff, int x, int y1, int y2, unsigned int rgb)
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
    for (int y = y1; y <= y2; y++)
    {
        int row_start_in_buff = y * buff.width;
        buff.buffer[row_start_in_buff + x] = rgb;
    }
}


void kev_render_line_low_slope(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb)
{

    int dx = x2 - x1;
    int dy = y2 - y1;
    int y_inc = 1;
    if (dy < 0)
    {
        y_inc = -1;
        dy = 0 - dy;
    }
    int diff = (2 * dy) - dx;
    int last_drawn_x = x1;
    int x = x1;
    for (; x <= x2; x++)
    {
        if (diff > 0)
        {
            //printf("dx %d dy %d diff %d last_drawn %d to %d\n", dx, dy, diff, last_drawn_x, x);
            kev_render_horiz_line(buff, last_drawn_x, x, y1, rgb);
            last_drawn_x = x;
            y1 += y_inc;
            diff += (2 * (dy - dx));
            //printf("diff %d\n", diff);
        }
        else
        {
            diff += (2 * dy);
        }
    }
    if (x > last_drawn_x)
    {
        kev_render_horiz_line(buff, last_drawn_x, x, y1, rgb);
    }
}

void kev_render_line_high_slope(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb)
{

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_inc = 1;
    if (dx < 0)
    {
        x_inc = -1;
        dx = 0 - dx;
    }
    int diff = (2 * dx) - dy;
    int last_drawn_y = y1;
    int y = y1;
    for (; y <= y2; y++)
    {
        if (diff > 0)
        {
            //printf("dx %d dy %d diff %d last_drawn %d to %d\n", dx, dy, diff, last_drawn_y, y);
            kev_render_vert_line(buff, x1, last_drawn_y, y, rgb);
            last_drawn_y = y;
            x1 += x_inc;
            diff += (2 * (dx - dy));
            //printf("diff %d\n", diff);
        }
        else
        {
            diff += (2 * dx);
        }
    }
    if (y > last_drawn_y)
    {
        kev_render_vert_line(buff, x1, last_drawn_y, y, rgb);
    }
}

void kev_render_line(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb)
{

    if (y2 == y1) kev_render_horiz_line(buff, x1, x2, y1, rgb);
    if (x2 == x1) kev_render_vert_line(buff, x1, y1, y2, rgb);
    if (abs(y2 - y1) < (abs(x2 - x1)))
    {
        if (x1 < x2)
        {
            kev_render_line_low_slope(buff, x1, y1, x2, y2, rgb);
        }
        else
        {
            kev_render_line_low_slope(buff, x2, y2, x1, y1, rgb);
        }
    }
    else
    {
        if (y1 < y2)
        {
            kev_render_line_high_slope(buff, x1, y1, x2, y2, rgb);
        }
        else
        {
            kev_render_line_high_slope(buff, x2, y2, x1, y1, rgb);
        }
    }
}

void kev_render_point(kev_render_buffer buff, int x, int y, unsigned int rgb)
{
    buff.buffer[y * buff.width + x] = rgb;
}

void kev_render_int(kev_render_buffer buff, int x, int y, int digit_width, int digit_height, int number, unsigned int rgb)
{
    char formatted[50];
    snprintf(formatted, 50, "%d", number);
    int i = 0;
    while (formatted[i] != 0)
    {
        kev_render_digit(buff, x + i * digit_width, y, 20, 40, formatted[i], rgb);
        i++;
    }
}

void kev_render_rectangle(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb)
{
    kev_render_horiz_line(buff, x1, x2, y1, rgb);
    kev_render_vert_line(buff, x2, y1, y2, rgb);
    kev_render_horiz_line(buff, x2, x1, y2, rgb);
    kev_render_vert_line(buff, x1, y2, y1, rgb);
}
void kev_render_digit(kev_render_buffer buff, int x, int y, int width, int height, char digit, unsigned int rgb)
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
        kev_render_horiz_line(buff, x + left, x + right, y + top, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom, rgb);
        kev_render_vert_line(buff, x + left, y + bottom, y + top, rgb);
        break;

    case '1':
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        break;
    case '2':
        kev_render_horiz_line(buff, x + left, x + right, y + top, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + mid, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + mid, rgb);
        kev_render_vert_line(buff, x + left, y + mid, y + bottom, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + bottom, rgb);
        break;
    case '3':
        kev_render_horiz_line(buff, x + left, x + right, y + top, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + mid, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + mid, rgb);
        kev_render_vert_line(buff, x + right, y + mid, y + bottom, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom, rgb);
        break;
    case '4':
        kev_render_vert_line(buff, x + left, y + top, y + mid, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + mid, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        break;
    case '5':
        kev_render_horiz_line(buff, x + right, x + left, y + top, rgb);
        kev_render_vert_line(buff, x + left, y + top, y + mid, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + mid, rgb);
        kev_render_vert_line(buff, x + right, y + mid, y + bottom, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom, rgb);
        break;
    case '6':
        kev_render_horiz_line(buff, x + right, x + left, y + top, rgb);
        kev_render_vert_line(buff, x + left, y + top, y + bottom, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + bottom, rgb);
        kev_render_vert_line(buff, x + right, y + bottom, y + mid, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + mid, rgb);
        break;
    case '7':
        kev_render_horiz_line(buff, x + left, x + right, y + top, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        break;
    case '8':
        kev_render_horiz_line(buff, x + left, x + right, y + top, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        kev_render_horiz_line(buff, x + right, x + left, y + bottom, rgb);
        kev_render_vert_line(buff, x + left, y + bottom, y + top, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + mid, rgb);
        break;
    case '9':
        kev_render_horiz_line(buff, x + right, x + left, y + top, rgb);
        kev_render_vert_line(buff, x + left, y + top, y + mid, rgb);
        kev_render_horiz_line(buff, x + left, x + right, y + mid, rgb);
        kev_render_vert_line(buff, x + right, y + top, y + bottom, rgb);
        break;
    }
}
