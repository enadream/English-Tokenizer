#include "clock.hpp"
#include "misc/util.hpp"

Clock::Clock() {
}

Clock::~Clock() {
}

void Clock::StartTimer() {
	start_time = std::chrono::high_resolution_clock::now();
}

void Clock::EndTimer() {
	end_time = std::chrono::high_resolution_clock::now();
}

long long Clock::GetDuration(const char& type) { // s:seconds, ms:mili sec, us: micro sec, ns: nano sec,
	if (util::DoesContain("ms", &type)) // Milliseconds
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	else if (util::DoesContain("us", &type)) // Microseconds
		duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	else if (util::DoesContain("ns", &type)) // Nanoseconds
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
	else // seconds
		duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	return duration.count();
}