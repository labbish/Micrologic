#include "BlockC.h"

bool BlockC::check() {
	for (Line* l : this->inputLines) if (l->mode != LINE) return false;
	for (Line* l : this->outputLines) if (l->mode != WIDELINE) return false;
	return this->inputLines.size() == 4 && this->outputLines.size() == 1;
}

void BlockC::tick() {
	for (int i = 0; i < 4; i++) {
		this->outputLines[0]->nextWideValue[i] = this->inputLines[i]->value;
	}
}