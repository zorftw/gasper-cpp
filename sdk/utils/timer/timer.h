#pragma once

class c_timer {
private:
	unsigned long long time;
public:
	c_timer();
	c_timer(unsigned long long time) : time(time) {}

	void reset();
	unsigned long long get_time();
	bool has_passed(long ms);
};