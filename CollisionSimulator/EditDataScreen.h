#pragma once
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "Screen.h"
#include "Consts.h"
#include "Logger.h"
#include "DataService.h"
#include "EventEmitter.h"

#define eds_FONT_S 50

#include <stdio.h>
static std::string dToString(double var)
{
	char str[64];
	sprintf_s(str, "%.3f", var);
	return std::string(str);
}

class EditDataScreen : public Screen {
private:
	ALLEGRO_FONT* _defaultFont;
	ALLEGRO_FONT* _standardFont;
	ALLEGRO_FONT* _boldFont;

	EventEmmiter<int>* _changeScreen$;
	DataService* _dataService; 

	Logger l;
private:
	int _x = 0;
	int _y = 0;
	int _selectedIndex = 0;
	std::string _editedValue;
	bool _isOrginal = true; // determines wheter the value in selectedIndex box has changed
public:
	EditDataScreen(ALLEGRO_FONT* font, DataService* dataService, EventEmmiter<int>* changeScreen$) 
	{
		_defaultFont = font;
		_standardFont = al_load_ttf_font(FONT_NAME, eds_FONT_S, 0);
		_boldFont = al_load_ttf_font(BOLD_FONT_NAME, eds_FONT_S, 0);

		_dataService = dataService;
		_changeScreen$ = changeScreen$;
	}
	~EditDataScreen() {
		al_destroy_font(_standardFont);
		al_destroy_font(_boldFont);
	}

	void Render() override {
		#define r_OFFSETX SCALED_WIDTH / 15
		#define r_OFFSETY SCALED_HEIGHT / 10
		#define r_WIDTH  r_OFFSETX * 13
		#define r_HEIGHT  r_OFFSETY * 7

		al_draw_rectangle(r_OFFSETX, r_OFFSETY, r_OFFSETX * 14, r_OFFSETY * 8, c_MAIN, 5);
		al_draw_text(_defaultFont, c_MAIN, r_OFFSETX, r_OFFSETY - DEFAULT_FONT_SIZE - 35, 0, "Edytuj dane:");

		renderLegend();
		
		for (int i = 0; i < 7; i++) {
			#define j ((i % 4)+1)
			const int x = i < 4 ? 1 : 3;

			const ALLEGRO_COLOR color = i == _selectedIndex ? c_ACCENT : c_MAIN;
			
			const std::string value = getCellValue(i);
			#define unit _dataService->data[i].GetUnit().c_str()
			const int unitWidth = al_get_text_width(_standardFont, unit);

			const int x1 = r_OFFSETX + r_WIDTH / 17 * (j + (j - 1) * 3);
			const int x2 = r_OFFSETX + r_WIDTH / 17 * j * 4;
			const int y1 = r_OFFSETY + r_HEIGHT / 5 * x;
			const int y2 = r_OFFSETY + r_HEIGHT / 5 * (x + 1);

			al_draw_rectangle(x1, y1, x2, y2, c_MAIN, 5); // draw input rect
			al_draw_text(_standardFont, c_MAIN, x1, y1 - eds_FONT_S - 35, 0,
				(_dataService->data[i].GetName()+":").c_str()); // draw input name
			al_draw_text(_standardFont, color, x1 + 20, y1 + (y2 - y1)/2 - eds_FONT_S/2 - 5, 0,
				value.c_str()); //draw value
			al_draw_text(_standardFont, color, x2 - 20 - unitWidth, y1 + (y2 - y1) / 2 - eds_FONT_S / 2 - 5, 0,
				unit); //draw unit

			#undef unit
		}		
	}
	void HandleKeyboardEvents(unsigned char* key) override {
		if (key[ALLEGRO_KEY_ESCAPE]) {
			saveEditedValue();
			_changeScreen$->Next(0);
			return;
		}

		if (key[ALLEGRO_KEY_R] &= KEY_RELEASED) {
			saveEditedValue();
			_dataService->Reset();
			return;
		}		

		#define R1 4
		#define R2 3

		if (key[ALLEGRO_KEY_RIGHT] &= KEY_RELEASED) {
			if ((_x < R1-1 && _y == 0) || (_x < R2-1 && _y == 1)) _x++;
		}
		else if (key[ALLEGRO_KEY_LEFT] &= KEY_RELEASED) {
			if ((_x > 0 && _y == 0) || (_x > 0 && _y == 1)) _x--;
		}
		else if (key[ALLEGRO_KEY_UP] &= KEY_RELEASED) {
			if (_y > 0) _y--;
		}
		else if (key[ALLEGRO_KEY_DOWN] &= KEY_RELEASED) {
			if (_y < 1) _y++;
		}
		else {
			handleInputToCell(key);
		}

		if (_y == 1 && _x == R1 - 1) _x = R2 - 1;

		const int nIndex = _x + _y*R1;
		if (_selectedIndex != nIndex) {
			saveEditedValue();

			_selectedIndex = nIndex;
			_isOrginal = true;
		}

		#undef R1
		#undef R2
	}
private:
	void handleInputToCell(unsigned char* key) {
		const std::string oldValue = _editedValue;
		char inputNumber = -1;
		std::string appendCharacter = "";

		if ((key[ALLEGRO_KEY_MINUS] &= KEY_RELEASED) || (key[ALLEGRO_KEY_PAD_MINUS] &= KEY_RELEASED)) {
			if (_editedValue.size() == 0) {
				appendCharacter = "-";
			}
			goto afterLoop;
		}
		if ((key[ALLEGRO_KEY_FULLSTOP] &= KEY_RELEASED) || (key[ALLEGRO_KEY_PAD_DELETE] &= KEY_RELEASED)) {
			if(_editedValue.find('.') == std::string::npos) appendCharacter = ".";
			goto afterLoop;
		}
		if (key[ALLEGRO_KEY_BACKSPACE] &= KEY_RELEASED) {
			if (_editedValue.size() == 0) return;
			_editedValue.pop_back();
			goto afterLoop;
		}		
				
		for (char i = 0; i <= 9; i++) {
			if ((key[ALLEGRO_KEY_0 + i] &= KEY_RELEASED) || (key[ALLEGRO_KEY_PAD_0 + i] &= KEY_RELEASED)) {
				inputNumber = i;
				break;
			}
		}

		if (inputNumber != -1) { 
			appendCharacter = std::to_string(inputNumber);
		}
	afterLoop:
		if (appendCharacter != "" && _editedValue.size() < 5) {
			_editedValue += appendCharacter;
		}

		if (oldValue != _editedValue) {
			_isOrginal = false;
		}
	}
	std::string getCellValue(int i) {
		const std::string val = dToString(_dataService->data[i].GetValue()).substr(0,5);
		if(i != _selectedIndex) return val;

		if (_isOrginal) {
			_editedValue = val;
			return val;
		}

		return _editedValue;
	}
	void saveEditedValue() {
		if (!_isOrginal) {
			try {
				// if data was changed try to set new value in data service 
				_dataService->data[_selectedIndex].SetValue(std::stod(_editedValue));
			}
			catch (...) {}

			_isOrginal = true;
		}
	}
	void renderLegend() {
		#define l_STARTY r_OFFSETY + r_HEIGHT + 20
		#define l_MARGIN 10

		auto drawLegendKey = [this](std::string key, std::string desc, int x, int y) -> void {
			const int t_width = al_get_text_width(_boldFont, key.c_str());
			al_draw_text(_boldFont, c_ACCENT, x, y, 0, key.c_str());
			al_draw_text(_standardFont, c_MAIN, x + t_width, y, 0, (" - " + desc).c_str());
		};

		drawLegendKey("R", "reset wartosci", r_OFFSETX, l_STARTY);
		drawLegendKey("Strzalki", "poruszanie sie", r_OFFSETX, l_STARTY + eds_FONT_S + l_MARGIN);
		drawLegendKey("Esc", "powrot do menu", r_OFFSETX + r_WIDTH / 2, l_STARTY);
	} 
};