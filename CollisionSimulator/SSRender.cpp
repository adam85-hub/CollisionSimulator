#include "SimulationScreen.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"

#include "Consts.h"

#define CX SCALED_WIDTH/2 // x center coord
#define CY SCALED_HEIGHT/2 // y center coord

#define COLOR1 al_map_rgb(87, 178, 169)
#define COLOR2 al_map_rgb(237, 106, 94)

static void draw_dotted_line(float, float, float, float, ALLEGRO_COLOR, float);

#include <stdio.h>
static std::string fToString(float var)
{
	char str[64];
	sprintf_s(str, "%.3f", var);
	return std::string(str);
}

void SimulationScreen::Render() {
#pragma region Skalowanie-w³aœciwoœci
	// u_ - unscaled, orginal value
	const float u_v1 = _simulationProps["v1"];
	const float v1 = u_v1 * a_SCALE;
	const float d = _simulationProps["d"] * a_SCALE;
	const float r1 = _simulationProps["r1"] * a_SCALE;
	const float r2 = _simulationProps["r2"] * a_SCALE;
	const float ox_zd = _simulationProps["ox_zd"] * a_SCALE; // odleg³oœæ w osi x œrodków kul w momencie zderzenia
	const float v3x = _simulationProps["v3x"] * a_SCALE;
	const float v3y = _simulationProps["v3y"] * a_SCALE;
	const float v4x = _simulationProps["v4x"] * a_SCALE;
	const float v4y = _simulationProps["v4y"] * a_SCALE;
#pragma endregion

#pragma region Kule
	// trajektoria lotu kuli 1
	draw_dotted_line(0, CY - d, CX - ox_zd, CY - d, COLOR1, 1);
	draw_dotted_line(CX - ox_zd, CY - d, CX - ox_zd + _simulationProps["v3drawx"], CY - d + _simulationProps["v3drawy"], COLOR1, 1);

	// trajektoria lotu kuli 2
	draw_dotted_line(CX, CY, CX + _simulationProps["v4drawx"], CY + _simulationProps["v4drawy"], COLOR2, 1);

	// kula 1
	if (_frameCount / FPS * v1 <= CX - ox_zd) {
		al_draw_filled_circle(_frameCount / FPS * v1, CY - d, r1, COLOR1);
		// kula 2
		al_draw_filled_circle(CX, CY, r2, COLOR2);
	}
	else {
		float t2 = _frameCount / FPS - ((CX - ox_zd) / v1);
		al_draw_filled_circle(CX - ox_zd + v3x * t2, CY - d + v3y * t2, r1, COLOR1);
		//kula 2
		al_draw_filled_circle(CX + v4x * t2, CY + v4y * t2, r2, COLOR2);
	}
#pragma endregion

#pragma region Skala
#define margin 20
#define L 5 // iloœæ kresek bez pocz¹tkowej(0m)
#define S 1 // dok³adnoœæ w metrach
#define width a_SCALE * L * S
#define startXPos CX - width/2
	al_draw_line(startXPos, SCALED_HEIGHT - margin, startXPos + width, SCALED_HEIGHT - margin, c_MAIN, 3);
	for (int i = 0; i < L * S + 1; i += S) {
		al_draw_line(startXPos + a_SCALE * i, SCALED_HEIGHT - margin, startXPos + a_SCALE * i, SCALED_HEIGHT - margin - 30, c_MAIN, 1);
		al_draw_text(_smallFont, c_MAIN, startXPos + a_SCALE * i, SCALED_HEIGHT - margin - _smallFontSize - 40, ALLEGRO_ALIGN_CENTER,
			(std::to_string(i) + "m").c_str());
	}
#undef startPos
#undef width
#undef margin
#undef L
#undef S
#pragma endregion

#pragma region Legenda
	auto drawLegendKey = [this](std::string key, std::string desc, int x, int y) -> void {
		const int t_width = al_get_text_width(_mediumBoldFont, key.c_str());
		al_draw_text(_mediumBoldFont, c_ACCENT, x, y, 0, key.c_str());
		al_draw_text(_mediumFont, c_MAIN, x + t_width, y, 0, (" - " + desc).c_str());
	};

#define Padding 30
	al_draw_text(_mediumBoldFont, c_ACCENT, Padding, SCALED_HEIGHT - Padding - 15 - _mediumFontSize * 4, 0, "Legenda:");
	drawLegendKey("Strzalki", "predkosc animacji", Padding, SCALED_HEIGHT - Padding - 10 - _mediumFontSize * 3);
	drawLegendKey("R", "resetuj animacje", Padding, SCALED_HEIGHT - Padding - 5 - _mediumFontSize*2);
	drawLegendKey("Spacja", "pauza", Padding, SCALED_HEIGHT - Padding - _mediumFontSize);
#undef Padding
#pragma endregion

#pragma region Pauza
#define pWidth 15
#define pPadding 20
#define pHeight _pauseHeight
	if (_pause) {
		// Ikona pauzy
		al_draw_filled_rectangle(SCALED_WIDTH - pWidth - pPadding, pPadding, SCALED_WIDTH - pPadding, pPadding + pHeight, c_ACCENT);
		al_draw_filled_rectangle(SCALED_WIDTH - pWidth * 2 - pPadding - 10, pPadding,
			SCALED_WIDTH - pPadding - pWidth - 10, pPadding + pHeight, c_ACCENT);

 		for (int i = 0; i < _dataToDisplay.size(); i++) {
			al_draw_text(_smallFont, c_MAIN, SCALED_WIDTH - 300, 70 + _smallFontSize*i, 0, _dataToDisplay[i].c_str());
		}
	}
	else {
#define tWidth 3*pWidth+10
		al_draw_filled_triangle(SCALED_WIDTH - pPadding - tWidth, pPadding,
			SCALED_WIDTH - pPadding - tWidth, pPadding + pHeight,
			SCALED_WIDTH - pPadding, pPadding + pHeight / 2, c_ACCENT);
	}
	// Prêdkoœæ animacji
	auto animSpeed = fToString(_animationSpeed) + "x";
	al_draw_text(_animSpeedFont, c_MAIN, SCALED_WIDTH - pPadding - tWidth - al_get_text_width(_animSpeedFont, animSpeed.c_str()) - 15,
		pPadding - 5, 0, animSpeed.c_str());
#undef tWidth
#undef pWidth
#undef pPadding
#undef pHeight
#pragma endregion

	// Obs³uga up³ywu czasu
	if (!_pause) 
		_frameCount+=_animationSpeed;
	if (_reset) {
		_frameCount = 0;
		_reset = false;
	}
}

static void draw_dotted_line(float x1, float y1, float x2, float y2, ALLEGRO_COLOR color, float thickness) {
	// podpowiedŸ: równanie prostej przechodz¹cej przez dwa punkty
#define spacing 8
	const float a = abs((y1 - y2) / (x1 - x2));
	const float cosalfa = cos(atan(a));
	const int count = sqrtf(powf(x1 - x2, 2) + powf(y1 - y1, 2))/spacing;
	const char znaky = y1 >= y2 ? -1 : 1;
	const char znakx = x1 >= x2 ? -1 : 1;

	for (int i = 0; i < count; i++) {
		if (i % 3 != 0) {
#define dx spacing * cosalfa * i
#define dx2 spacing * cosalfa * (i+1)
			al_draw_line(x1 + dx * znakx, y1 + a * dx * znaky, x1 + dx2 * znakx, y1 + dx2 * a * znaky, color, thickness);
#undef dx
#undef dx2
		}
	}
#undef spacing
}