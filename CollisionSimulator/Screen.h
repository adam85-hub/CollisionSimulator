#pragma once
#include "NotImplementedException.h"

class Screen {
public:
	virtual void Render() {
		throw new NotImplementedException();
	};
	virtual void HandleKeyboardEvents(unsigned char* key) {
		throw new NotImplementedException();
	};
};