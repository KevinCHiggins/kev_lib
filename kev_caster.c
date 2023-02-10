#include <math.h>
#include <stdio.h>
#include "kev_caster.h"

// assumes the point (x, y) is incident to a grid line, i.e. that
// !((x % 1 > 0) & (y % 1 > 0))
float get_face_incidence(kev_caster_grid grid, char ang_bit_field, float x, float y, kev_caster_material *mat)
{

	float x_integral_part;
	float y_integral_part;
	float x_fractional_part = modff(x, &x_integral_part);
	float y_fractional_part = modff(y, &y_integral_part);
	int x_int = (int)x_integral_part;
	int y_int = (int)y_integral_part;
	printf("x %f\n", x);
	printf("y %f\n", y);
	printf("x frac %f\n", x_fractional_part);
	printf("y frac %f\n", y_fractional_part);
	kev_caster_edge wall;

	if (x_fractional_part > y_fractional_part)
	{
		wall = kev_caster_point_at(grid, x_int, y_int).x_running;
		if (wall.vis)
		{
			if (ang_bit_field > 1 && ang_bit_field < 4)
			{
				*mat = wall.front;
				return x_fractional_part;
			}
			else if (ang_bit_field > 4)
			{
				*mat = wall.back;
				return 1 - x_fractional_part;
			}
		}
	}
	else if (y_fractional_part > x_fractional_part)
	{
		wall = kev_caster_point_at(grid, x_int, y_int).y_running;
		printf("Wall.vis at %d, %d: %d\n", x_int, y_int, wall.vis);
		if (wall.vis)
		{
			if (ang_bit_field < 3 || ang_bit_field == 7)
			{
				*mat = wall.front;
				return y_fractional_part;
			}
			else if (ang_bit_field > 3 && ang_bit_field < 6)
			{
				*mat = wall.back;
				return 1 - y_fractional_part;
			}
		}	
	}
	else
	{
		switch (ang_bit_field)
		{
		case 0: 
			wall = grid.data[(int)x, (int)y - 1].y_running;
			if (wall.vis)
			{
				*mat = wall.front;
				return 0;
			}
		case 1:
			wall = grid.data[(int)x, (int)y - 1].y_running;
			if (!wall.vis) wall = grid.data[(int)x, (int)y].x_running;
			if (wall.vis)
			{
				*mat = wall.front;
				return 0;
			}
		case 2: 
			wall = grid.data[(int)x, (int)y].x_running;
			if (wall.vis)
			{
				*mat = wall.front;
				return 0;
			}
		case 3:
			wall = grid.data[(int)x, (int)y].x_running;
			if (!wall.vis) wall = grid.data[(int)x, (int)y].y_running;
			if (wall.vis)
			{
				*mat = wall.front;
				return 0;
			}
		case 4: 
			wall = grid.data[(int)x, (int)y].y_running;
			if (wall.vis)
			{
				*mat = wall.back;
				return 0;
			}
		case 5:
			wall = grid.data[(int)x, (int)y].y_running;
			if (!wall.vis) wall = grid.data[(int)x - 1, (int)y].x_running;
			if (wall.vis)
			{
				*mat = wall.back;
				return 0;
			}
		case 6: 
			wall = grid.data[(int)x - 1, (int)y].x_running;
			if (wall.vis)
			{
				*mat = wall.back;
				return 0;
			}
		case 7:
			wall = grid.data[(int)x - 1, (int)y].x_running;
			if (!wall.vis) wall = grid.data[(int)x - 1, (int)y].y_running;
			if (wall.vis)
			{
				*mat = wall.back;
				return 0;
			}
		}
	}
	return -1.0; // flag indicating no wall found
}