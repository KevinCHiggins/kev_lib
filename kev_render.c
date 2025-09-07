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

void kev_render_point(kev_render_buffer buff, int x, int y, unsigned int rgb)
{
    buff.buffer[y * buff.width + x] = rgb;
}

void kev_render_debug_rgb(unsigned int rgb)
{
    printf("%3d,%3d,%3d\n", (rgb & 0x00ff0000) >> 16, (rgb & 0x0000ff00) >> 8, (rgb & 0x000000ff));
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
    int dx = (x2 - x1) * 2;
    int dy = (y2 - y1) * 2;
    int y_inc;
    int last_drawn_x = x1;
    if (dx * dy > 0) y_inc = 1; else y_inc = -1;
    int slope_error = y_inc * dx / 2;
    for (;x1 <= x2; x1++)
    {
        slope_error += dy;
        if (abs(slope_error) > dx)
        {
            slope_error -= y_inc * dx;
            kev_render_horiz_line(buff, last_drawn_x, x1 - 1, y1, rgb);
            last_drawn_x = x1;
            y1+= y_inc;
        }
        
    }
    kev_render_horiz_line(buff, last_drawn_x, x2, y2, rgb); // use x2 as x1 has overshot it

}

void kev_render_line_high_slope(kev_render_buffer buff, int x1, int y1, int x2, int y2, unsigned int rgb)
{
    int dx = (x2 - x1) * 2;
    int dy = (y2 - y1) * 2;
    int x_inc;
    int last_drawn_y = y1;
    if (dx * dy > 0) x_inc = 1; else x_inc = -1;
    int slope_error = x_inc * dy / 2;
    for (;y1 <= y2; y1++)
    {
        // int y = round(y1 + slope * (x - x1));
        slope_error += dx;
        if (abs(slope_error) > dy)
        {
            slope_error -= x_inc * dy;
            kev_render_vert_line(buff, x1, last_drawn_y, y1 - 1, rgb);
            last_drawn_y = y1;
            x1+= x_inc;
        }


    }
    kev_render_vert_line(buff, x2, last_drawn_y, y2, rgb); // use y2 as y1 has overshot it

}
void kev_render_float_line(kev_render_buffer buff, float x1, float y1, float x2, float y2, unsigned int rgb)
{

    float run = x2 - x1;
    float rise = y2 - y1; // in screen space... so this "rise" goes down
    float pixel_rounded_run = roundf(run);
    float pixel_rounded_rise = roundf(rise);


    if (pixel_rounded_run == 0.0)
    {
        kev_render_vert_line(buff, (int)roundf(x1), (int)roundf(y1), (int)roundf(y2), rgb);	
    }
    else if (pixel_rounded_rise == 0.0)
    {
        kev_render_horiz_line(buff, (int)roundf(x1), (int)roundf(x2), (int)roundf(y1), rgb);
    }
    else
    // low slope, positive rise, positive/negative run for the moment
    {

        float column = roundf(x1);  // we always draw given start pixel, although it may not be most accurate for line length or position
        float row = roundf(y1);     // same
        float end_column = roundf(x2);
        float start_of_current_span = column; // span is what gets drawn as horiz line
        float error_per_column = fabs(rise/run);
        int x_step = (run / fabs(run));
        int y_step = (rise / fabs(rise));
        // to get y error (difference) at centre of first pixel: slope by x difference *to* centre of start column in direction of x_step, shifted by y difference from centre of start row
        float error = (error_per_column * (x_step * (column - x1))) + (y_step * (y1 - row));
        while (column != end_column)
        {

            error += error_per_column;
            if (error > 0.5)
            {
                kev_render_horiz_line(buff, start_of_current_span, column, row, rgb);

                start_of_current_span = column + x_step;
                row += y_step;
                error--;
            }
            column += x_step;
            printf("error %f\n", error);
        };
        // check for case where algorithm hasn't reached last pixel's row
        float end_row = roundf(y2);
        if (row == end_row - y_step)
        {
            kev_render_horiz_line(buff, start_of_current_span, column - x_step, row, rgb);
            start_of_current_span = column;

        }
        kev_render_horiz_line(buff, start_of_current_span, column, end_row, rgb); // using end_row ensures we don't draw past it (when the line, if continued would hit column centre one row further than end row)
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


void kev_render_fill(kev_render_buffer buff, unsigned int rgb)
{
    for (int i = 0; i < buff.width * buff.height; i++)
    {
    
        buff.buffer[i] = rgb;
    }
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

void kev_render_img(kev_render_buffer buff, int x, int y, int width, int height, kev_render_buffer img)
{
    for (int row = y; row < y + height; row++)
    {
        for (int pixel = x; pixel < x + width; pixel++)
        {
            buff.buffer[(row * buff.width) + pixel] = img.buffer[(row - y) * width + (pixel - x)];
        }
    }

}

void kev_render_stretched_img_slice(kev_render_buffer buff, int x, int top_y, int bottom_y, float offset, kev_render_buffer img)
{
    float height_stretched = bottom_y - top_y;
    // ignoring that sampling points should be mid-pixel for now
    // would also be nice do make it integer to match the Bresenham
    float fraction = img.height / height_stretched;
    float vertical_offset = 0.0;
    offset = img.width * offset;
    for (; top_y < bottom_y; top_y++)
    {


        if (top_y >= 0 & top_y < buff.height) // TO-DO only do necessary loop iterations
        {
            kev_render_point(buff, x, top_y, img.buffer[(int)(vertical_offset) * img.width + (int)offset]);
        }
        vertical_offset += fraction;
        
    }
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
