#pragma once
#include <vector>
#include <string>
#include "Screen.h"
#include <allegro5/allegro_font.h>
#include "Consts.h"
#include "EventEmitter.h"

#include <iostream>

#define SF_SIZE 30

class MenuScreen : public Screen {
private: 
	ALLEGRO_FONT* _defaultFont;
	ALLEGRO_FONT* _smallFont;
	ALLEGRO_FONT* _titleFont;
	const int _titleFontSize = 100;

	std::vector<std::string> _menuOptions;
	int _activeOption = 0;
	EventEmmiter<bool>* _exit$;
	EventEmmiter<int>* _changeScreen$;
public:
	MenuScreen(ALLEGRO_FONT* font, EventEmmiter<bool>* exit$, EventEmmiter<int>* changeScreen$) {
		_defaultFont = font;
		_exit$ = exit$;
		_changeScreen$ = changeScreen$;
		_smallFont = al_load_ttf_font(FONT_NAME, SF_SIZE, 0);
		_titleFont = al_load_ttf_font(FONT_NAME, _titleFontSize, 0);

		_menuOptions.push_back("Symulacja");
		_menuOptions.push_back("Zmiana danych");
		_menuOptions.push_back("Wyjscie");
	}
	void Render() override {
		//Menu options
		for (int i = 0; i < _menuOptions.size(); i++) {
			al_draw_text(_defaultFont, _activeOption == i ? c_ACCENT : c_MAIN, 
				SCALED_WIDTH / 2, SCALED_HEIGHT/6 * (i + 2) - DEFAULT_FONT_SIZE/2, 
				ALLEGRO_ALIGN_CENTRE, _menuOptions[i].c_str());
		}

		// Signature
		#define PODPIS "Adam Bialik 2022"
		const int tWidth = al_get_text_width(_smallFont, PODPIS);
		al_draw_text(_smallFont, c_ACCENT, SCALED_WIDTH - tWidth - 25, SCALED_HEIGHT - SF_SIZE - 25, 0, PODPIS);
		#undef PODPIS
	}
	void HandleKeyboardEvents(unsigned char* key) override {
		if (key[ALLEGRO_KEY_DOWN] &= KEY_RELEASED) {
			if (_activeOption < _menuOptions.size()-1) _activeOption++;
		}
		else if (key[ALLEGRO_KEY_UP] &= KEY_RELEASED) {			
			if (_activeOption > 0) _activeOption--;
		}	
		else if (key[ALLEGRO_KEY_ENTER] &= KEY_RELEASED) {
			MenuOptionSelected(_activeOption);
		}
	}
private:
	void MenuOptionSelected(int selectedIndex) {
		switch (selectedIndex)
		{
			case 0:
				_changeScreen$->Next(2);
				break;
			case 1:				
				_changeScreen$->Next(1);
				break;
			case 2: 
				_exit$->Next(true);
				break;			
			default:
				throw std::runtime_error("MenuScreen::MenuOptionSelected: Menu option with index " + std::to_string(selectedIndex) + "does not exist");
				break;
		}
	}
};

#undef SF_SIZE