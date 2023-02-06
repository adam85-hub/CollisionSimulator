#pragma once
#include <string>
#include <iostream>

class Logger {
public:
	void Info(std::string msg) {
		std::cout << msg << std::endl;
	}

	void Error(std::string msg) {
		std::cout << msg << std::endl;
	}
};