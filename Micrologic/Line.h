#pragma once

#include <cstdio>

class Line {
public:
	bool nextValue;
	bool value;
	Line();
	void set(bool value);
	void flush();
};
