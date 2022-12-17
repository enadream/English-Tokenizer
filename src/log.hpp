#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <windows.h>

// User libs
#include "misc/data_types.hpp"

class Log {
public:
	template <typename Type>
	static Log Warning(const Type& message);

	template <typename Type>
	static Log Error(const Type& message);

	template <typename Type>
	static Log Info(const Type& message);

	template <typename Type>
	Log operator<<(const Type& message);
};


static Log s_log;

//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Red
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors


static inline void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

template <typename Type>
Log Log::Warning(const Type& message) {
	SetColor(6);
	std::cout << "[WARNING]: ";
	SetColor(7);
	std::cout << message;
	return s_log;
}

template <typename Type>
Log Log::Info(const Type& message) {
	SetColor(2);
	std::cout << "[INFO]: ";
	SetColor(7);
	std::cout << message;
	return s_log;
}

template <typename Type>
Log Log::Error(const Type& message) {
	SetColor(4);
	std::cout << "[ERROR]: ";
	SetColor(7);
	std::cout << message;
	return s_log;
}

template <typename Type>
Log Log::operator<<(const Type& message) {
	std::cout << message;
	return s_log;
}

#endif
