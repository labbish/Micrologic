#pragma once

#include <vector>
#include "Line.h"

class Block {
public:
	std::vector<Line*> inputLines;
	std::vector<Line*> outputLines;
	Block();
	Block(std::vector<Line*> inputLines, std::vector<Line*> outputLines);
	virtual bool check() = 0;
	virtual void tick() = 0;
};
