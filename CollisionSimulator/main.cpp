#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include "CollisionSimulator.h"

int main() {
	CollisionSimulator* collisionSimulator = new CollisionSimulator();
	
	return collisionSimulator->Execute();
}