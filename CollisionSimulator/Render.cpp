#include "CollisionSimulator.h"
#include "Consts.h"

void CollisionSimulator::Render() {
	const float scale_factor_x = ((float)al_get_display_width(_display)) / SCALED_WIDTH;
	const float scale_factor_y = ((float)al_get_display_height(_display)) / SCALED_HEIGHT;
	
	al_clear_to_color(al_map_rgb(0, 0, 0));

	_program->Render();

	al_identity_transform(&_transform);		
	al_scale_transform(&_transform, scale_factor_x, scale_factor_y);
	al_use_transform(&_transform);

	al_flip_display();
}