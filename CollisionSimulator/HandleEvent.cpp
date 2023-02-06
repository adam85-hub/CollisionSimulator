#include "CollisionSimulator.h"

void CollisionSimulator::HandleEvent(ALLEGRO_EVENT* event) {
	switch (event->type)
	{
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			_running = false;
			break;
		default:
			break;
	}
}