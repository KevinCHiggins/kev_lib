#include <stddef.h>

typedef struct {
	unsigned texture_id;
	int x_scale;
	int y_scale;
	int offset;
} kev_caster_material;

typedef struct {
	int vis;
	kev_caster_material front;
	kev_caster_material back;
} kev_caster_edge;

typedef struct {
	kev_caster_edge x_running;
	kev_caster_edge y_running;
} kev_caster_grid_point;

typedef struct {
	size_t x_size;
	size_t y_size;
	kev_caster_grid_point *data;
} kev_caster_grid;

float get_face_incidence(kev_caster_grid grid, char ang_bit_field, float x, float y, kev_caster_material *mat);

#define kev_caster_point_at(grid, x, y) grid.data[y * grid.x_size + x]