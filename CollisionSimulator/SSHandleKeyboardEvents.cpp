#include "SimulationScreen.h"
#include "allegro5/allegro5.h"

void SimulationScreen::HandleKeyboardEvents(unsigned char* key) {
	if (key[ALLEGRO_KEY_ESCAPE]) {
		_changeScreen$->Next(0);
		return;
	}

	if (key[ALLEGRO_KEY_SPACE] &= KEY_RELEASED) {
		_pause = !_pause;
	}

	if (key[ALLEGRO_KEY_R] &= KEY_RELEASED) {
		_reset = true;
	}

	if (key[ALLEGRO_KEY_RIGHT] &= KEY_RELEASED) {
		if (_animationSpeed >= 0.125) _animationSpeed *= 2;
		else if ( _animationSpeed == -0.125) _animationSpeed *= -1;
		else _animationSpeed /= 2;
	}
	else if (key[ALLEGRO_KEY_LEFT] &= KEY_RELEASED) {
		if (_animationSpeed > 0.125) _animationSpeed /= 2;
		else if (_animationSpeed == 0.125) _animationSpeed *= -1;
		else _animationSpeed *= 2;
	}
}