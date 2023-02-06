#pragma once
#include <vector>
#include <map>
#include "DataModel.h"

class DataService {
public:
	std::vector<DataModel> data;
public:
	DataService()
	{
		Reset();
	}

	void Reset() {
		data = std::vector<DataModel>();
		auto testFunction = [](double nValue) -> bool { return nValue > 0; };
		data.push_back(DataModel("m1", "kg", testFunction, 1));
		data.push_back(DataModel("v1", "m/s", testFunction, 3));
		data.push_back(DataModel("r1", "m", testFunction, 0.5));
		data.push_back(DataModel("d", "m", testFunction, 0.5));
		data.push_back(DataModel("m2", "kg", testFunction, 1));
		data.push_back(DataModel("v2", "m/s", [](double) { return true; }, -3));
		data.push_back(DataModel("r2", "m", testFunction, 0.5));
	}

	const std::map<std::string, double> GetMap() {
		auto map = std::map<std::string, double>();
		for (DataModel d : data) {
			map.insert(std::pair(d.GetName(), d.GetValue()));
		}

		return map;
	}
};