#include "SimulationScreen.h"
#include <algorithm>

#define M_PI 3.14159265358979323846

static float fromR(float radians) {
	return radians * 180.0 / M_PI;
}

static float toR(float degrees) {
	return degrees * M_PI / 180.0;
}

static float getMBT(float, float);

void SimulationScreen::calculateSimulation() {
	auto addToProps = [this](std::string key, float value) -> void {
		_simulationProps.insert(std::pair(key, value));
	};
	auto map = _dataService->GetMap();	
	addToProps("v1", map["v1"] - map["v2"]);

	float cosAlfa = map["d"] / (map["r1"] + map["r2"]);
	float alfa = fromR(acos(cosAlfa));
	float beta = 90 - alfa;
	float v1a = _simulationProps["v1"] * sin(toR(alfa));
	float v1b = _simulationProps["v1"] * cos(toR(alfa));
	float v3a = ((map["m1"] * v1a) - (map["m2"] * v1a)) / (map["m1"] + map["m2"]);
	float v4 = 2 * map["m1"] * v1a / (map["m1"] + map["m2"]);
	float v3 = sqrt(v1b * v1b + v3a * v3a);
	float gamma = fromR(atan(v3a/v1b));	
	float psi = 90 - gamma; // ten du¿y k¹t
	float ox_zd = map["d"] / tan(toR(beta)); // odleg³oœæ œrodków kuli w osi x w momencie zderzenia
	
	float v3x = v3 * cos(toR(alfa - gamma));
	float v3y = -v3 * sin(toR(alfa - gamma));

	float v4x = v4 * cos(toR(beta));
	float v4y = v4 * sin(toR(beta));

	float v3scale = std::max(getMBT(v3x * a_SCALE, SCALED_WIDTH), getMBT(v3y * a_SCALE, SCALED_HEIGHT));
	float v4scale = std::max(getMBT(v4x * a_SCALE, SCALED_WIDTH), getMBT(v4y * a_SCALE, SCALED_HEIGHT));

	auto addToDisplay = [this](std::string a) -> void {
		_dataToDisplay.push_back(a);
	};

	addToDisplay("Wyliczone parametry:");
	addToDisplay("psi = " + std::to_string(psi) + " deg");
	addToDisplay("v3 = " + std::to_string(v3) + " m/s");
	addToDisplay("v4 = " + std::to_string(v4) + " m/s");
	addToDisplay("alfa = " + std::to_string(alfa) + " deg");
	addToDisplay("beta = " + std::to_string(beta) + " deg");
	addToDisplay("gamma = " + std::to_string(gamma) + " deg");

	addToProps("ox_zd", ox_zd);
	addToProps("r1", map["r1"]);
	addToProps("r2", map["r2"]);	
	addToProps("d", map["d"]);
	addToProps("v3", v3);
	addToProps("v3x", v3x);
	addToProps("v3y", v3y);
	addToProps("v4x", v4x);
	addToProps("v4y", v4y);
	addToProps("psi", psi);
	addToProps("alfa", alfa);
	addToProps("gamma", gamma);

	// naprawiæ problem z wydajnoœci¹ dla skrajnych przypadków (np. d=0.001) jedna ze wspó³rzêdnych jest wtedy bardzo du¿a
	addToProps("v3drawx", v3scale * v3x * a_SCALE);
	addToProps("v3drawy", v3scale * v3y * a_SCALE);
	addToProps("v4drawx", v4scale * v4x * a_SCALE);
	addToProps("v4drawy", v4scale * v4y * a_SCALE);
}

// Get min number bigger than max (w przybli¿eniu)
static float getMBT(float v, float max) { 
	return abs((max / v));
}

#undef M_PI