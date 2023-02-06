#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "Program.h"
#include "Consts.h"
#include "Logger.h"

class CollisionSimulator
{
private:
	ALLEGRO_TIMER* _timer; 
	ALLEGRO_EVENT_QUEUE* _queue;
	ALLEGRO_DISPLAY* _display;
	ALLEGRO_FONT* _defaultFont;
	ALLEGRO_TRANSFORM _transform;
	bool _running;
	Program* _program;
	Logger* _logger;
public:
	CollisionSimulator();
	int Execute();
private:
	void Init();
	void HandleEvent(ALLEGRO_EVENT*);
	void HandleKeyboardEvents(unsigned char*);
	void Update();
	void Loop();
	void Render();
	void CleanUp();
public:
	void Exit();
};

