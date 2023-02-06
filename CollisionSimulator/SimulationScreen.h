#pragma once
#include <map>
#include "allegro5/allegro_ttf.h"

#include "Screen.h"
#include "DataService.h"
#include "EventEmitter.h"
#include "Consts.h"
#include "Logger.h"

class SimulationScreen : public Screen {
private: 
	EventEmmiter<int>* _changeScreen$;
	ALLEGRO_FONT* _smallFont;
	const int _smallFontSize = 20;
	ALLEGRO_FONT* _mediumFont;
	ALLEGRO_FONT* _mediumBoldFont;
	const int _mediumFontSize = 30;
	ALLEGRO_FONT* _animSpeedFont;
	const int _pauseHeight = 40;

	DataService* _dataService;
	std::map<std::string, float> _simulationProps; // dane dla uk³adu odniesienia w którym v2=0
	std::vector<std::string> _dataToDisplay;
	double _frameCount;
	bool _pause = false;
	bool _reset = false;
	float _animationSpeed = 1;
public:
	SimulationScreen(DataService* dataService, EventEmmiter<int>* changeScreen$)
	{
		_dataService = dataService;
		_changeScreen$ = changeScreen$;
		_frameCount = 0;
		_smallFont = al_load_ttf_font(FONT_NAME, _smallFontSize, 0);
		_mediumFont = al_load_ttf_font(FONT_NAME, _mediumFontSize, 0);
		_mediumBoldFont = al_load_ttf_font(BOLD_FONT_NAME, _mediumFontSize, 0);
		_animSpeedFont = al_load_ttf_font(BOLD_FONT_NAME, _pauseHeight, 0);

		calculateSimulation();	
	}
	~SimulationScreen() {
		al_destroy_font(_smallFont);
		al_destroy_font(_mediumFont);
		al_destroy_font(_mediumBoldFont);
	}
	void Render() override;
	void HandleKeyboardEvents(unsigned char* key) override;
private:
	void calculateSimulation();
};

#undef smallFontSize