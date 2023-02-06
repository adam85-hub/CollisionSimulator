#pragma once
#include <string>

typedef bool (*TestFunction)(double);

class DataModel {
private: 
	std::string _name;
	double _value;
	std::string _unit;
	TestFunction _testFunction;
public:
	DataModel(std::string name, std::string unit, TestFunction testFunciton, double value = 0) {
		_value = value;
		_unit = unit;
		_name = name;
		_testFunction = testFunciton;
	}

	bool SetValue(double newValue) {
		if (_testFunction(newValue)) {
			_value = newValue;
			return true;
		}

		return false;
	}

	bool IsValid(double value) {
		return _testFunction(value);
	}

	double GetValue() {
		return _value;
	}

	std::string GetName() {
		return _name;
	}

	std::string GetUnit() {
		return _unit;
	}
};