#pragma once
#include <allegro5/allegro_font.h>
#include <vector>
#include <stdexcept>
#include <variant>
#include <functional>

#include "Screen.h"
#include "MenuScreen.h"
#include "EditDataScreen.h"
#include "SimulationScreen.h"

#include "EventEmitter.h"
#include "Logger.h"
#include "DataService.h"

using ScreenFn = std::function<Screen* ()>;
static struct ScreenVisitor {
	Screen* operator()(Screen* value) const {
		return value;
	}

	Screen* operator()(ScreenFn value) const {
		return value();
	}
};
static Screen* toScreen(std::variant<Screen*, ScreenFn> value) {
	return std::visit(ScreenVisitor{}, value);
}

class Program {
private:
	ALLEGRO_FONT* _defaultFont;

	Screen* _displayedScreen;
	std::vector<std::variant<Screen*, ScreenFn>> _screens;
	EventEmmiter<int> _changeScreen$;

	DataService _dataService;
	int _oldSelectedIndex;
public:
	EventEmmiter<bool> exit$;
public:
	Program(ALLEGRO_FONT* font, Logger* logger) {
		_defaultFont = font;

		_screens.push_back(new MenuScreen(_defaultFont, &exit$, &_changeScreen$));
		_screens.push_back(new EditDataScreen(_defaultFont, &_dataService, &_changeScreen$));
		_screens.push_back([this]() -> Screen* { return new SimulationScreen(&_dataService, &_changeScreen$); });
		//_screens.push_back(new SimulationScreen(&_dataService, &_changeScreen$));

		_displayedScreen = toScreen(_screens[0]);
		_oldSelectedIndex = 0;
	}
	~Program() {
		for (auto screen : _screens) {
			delete toScreen(screen);
		}
	}

	void Update() {
		std::optional<int> index = _changeScreen$.Emmited();
		if (index.has_value() == true) {
			ChangeScreen(index.value());
		}
	}

	void Render() {
		_displayedScreen->Render();
	}

	void HandleKeyboardEvents(unsigned char* key) {
		_displayedScreen->HandleKeyboardEvents(key);
	}

	void ChangeScreen(int screenIndex) {
		if (screenIndex < 0 || screenIndex >= _screens.size()) {
			throw std::runtime_error("Program::ChangeScreen: Screen with index " + std::to_string(screenIndex) + " does not exist.");
			return;
		}

		if (_screens[_oldSelectedIndex].index() == 1) delete _displayedScreen;

		_displayedScreen = toScreen(_screens[screenIndex]);
		_oldSelectedIndex = screenIndex;
	}
};