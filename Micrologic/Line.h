#pragma once

#include <cstdio>
#include <string>

enum {
	LINE = 0,
	WIDELINE = 1
};

class Line {
public:
	short mode;
	bool nextValue;
	bool value;
	bool nextWideValue[4];
	bool wideValue[4];
	Line(int mode = LINE);
	void set(bool value);
	void set(bool wideValue[4]);
	void flush();
	std::string checkValue();
};
