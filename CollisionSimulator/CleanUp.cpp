#include "CollisionSimulator.h"

void CollisionSimulator::CleanUp() {

	delete _program;
	delete _logger;

	al_destroy_font(_defaultFont);
	al_destroy_display(_display);
	al_destroy_timer(_timer);
	al_destroy_event_queue(_queue);
}