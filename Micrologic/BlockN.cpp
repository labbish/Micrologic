#include "BlockN.h"

bool BlockN::check() {
	return this->inputLines.size() == 1 && this->outputLines.size() == 1;
}

void BlockN::tick() {
	this->outputLines[0]->nextValue = !this->inputLines[0]->value;
}