#include "CollisionSimulator.h"

int CollisionSimulator::Execute() {
	Init();

	bool redraw = true;
	ALLEGRO_EVENT event;

	unsigned char key[ALLEGRO_KEY_MAX];
	memset(key, 0, sizeof(key));

	al_start_timer(_timer);
	while (_running) {
		al_wait_for_event(_queue, &event);	
		switch (event.type)
		{
			case ALLEGRO_EVENT_TIMER:
				HandleKeyboardEvents(key);
				Update();
				for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
					key[i] &= KEY_SEEN;

				redraw = true;
				break;

			case ALLEGRO_EVENT_KEY_DOWN:
				key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
				break;
			case ALLEGRO_EVENT_KEY_UP:
				key[event.keyboard.keycode] &= KEY_RELEASED;
				break;
			default:
				HandleEvent(&event);
				break;
		}

		Loop();

		if (redraw && al_is_event_queue_empty(_queue))
		{
			Render();
			redraw = false;
		}
	}
	
	CleanUp();
	return 0;
}