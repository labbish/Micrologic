#include "Line.h"

Line::Line() {
	this->value = 0;
	this->nextValue = 0;
}

void Line::set(bool value) {
	this->value = this->nextValue = value;
}

void Line::flush() {
	this->value = this->nextValue;
}