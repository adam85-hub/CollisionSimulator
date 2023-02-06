#include "CollisionSimulator.h"

void CollisionSimulator::Init() {
	al_init();
	al_install_keyboard();

	_timer = al_create_timer(1 / FPS);
	_queue = al_create_event_queue();
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); 
#define SCALE 80
	_display = al_create_display(16*SCALE, 9*SCALE);
#undef SCALE

	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	_defaultFont = al_load_ttf_font(FONT_NAME, DEFAULT_FONT_SIZE, 0);

	al_register_event_source(_queue, al_get_keyboard_event_source());
	al_register_event_source(_queue, al_get_display_event_source(_display));
	al_register_event_source(_queue, al_get_timer_event_source(_timer));

	_running = true;

	_logger = new Logger();
	_program = new Program(_defaultFont, _logger);
}