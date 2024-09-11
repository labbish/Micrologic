#include "BlockA.h"

bool BlockA::check() {
	return this->inputLines.size() == 2 && this->outputLines.size() == 1;
}

void BlockA::tick() {
	this->outputLines[0]->nextValue = this->inputLines[0]->value && this->inputLines[1]->value;
}