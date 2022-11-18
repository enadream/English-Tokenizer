#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <chrono>


// User libs
#include "datatypes.hpp"

class Clock {
private:
	std::chrono::high_resolution_clock::time_point start_time;
	std::chrono::high_resolution_clock::time_point end_time;
	std::chrono::nanoseconds duration;
public:
	Clock();
	~Clock();

	void StartTime();
	void EndTime();
	long long GetDuration(const char& type = *"s");
};




#endif // !CLOCK_H
