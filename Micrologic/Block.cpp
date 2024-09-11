#include "Block.h"

bool Echo = 1;

Block::Block() {
	this->inputLines = {};
}

Block::Block(std::vector<Line*> inputLines, std::vector<Line*> outputLines) {
	this->inputLines = inputLines;
	this->outputLines = outputLines;
}