#include "CollisionSimulator.h"

void CollisionSimulator::Update() {
	if (_program->exit$.Emmited() == true) {
		Exit();
	}

	_program->Update();
}