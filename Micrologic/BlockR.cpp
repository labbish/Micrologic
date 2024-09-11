#include "BlockR.h"

bool BlockR::check() {
	return this->inputLines.size() == 2 && this->outputLines.size() == 1;
}

void BlockR::tick() {
	this->outputLines[0]->nextValue = this->inputLines[0]->value && this->inputLines[1]->value;
}