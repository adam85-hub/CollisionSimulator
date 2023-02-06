#pragma once
#include <optional>

template<typename T>
class EventEmmiter {
private:
	std::optional<T> _object = std::nullopt;
public:
	void Next(T object) {
		_object = object;
	}

	std::optional<T> Emmited() {
		std::optional<T> buff = _object;
		_object = std::nullopt;
		return buff;
	}
};