#include "CollisionSimulator.h"

void CollisionSimulator::HandleKeyboardEvents(unsigned char *key) {
	//if (key[ALLEGRO_KEY_X]) { // only development delete in production code
	//	Exit();
	//}

	_program->HandleKeyboardEvents(key);
}