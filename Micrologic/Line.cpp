#include "Line.h"

Line::Line(int mode) {
	this->mode = mode;
	this->value = this->nextValue = 0;
	for (int i = 0; i < 4; i++) this->wideValue[i] = this->nextWideValue[i] = 0;
}

void Line::set(bool value) {
	this->value = this->nextValue = value;
}

void Line::set(bool wideValue[4]) {
	for (int i = 0; i < 4; i++) this->wideValue[i] = this->nextWideValue[i] = wideValue[i];
	for (int i = 0; i < 4; i++) printf("%d ", wideValue[i]);
	printf("\n");
}

void Line::flush() {
	this->value = this->nextValue;
	for (int i = 0; i < 4; i++) this->wideValue[i] = this->nextWideValue[i];
}

std::string Line::checkValue() {
	if (this->mode == LINE) return std::to_string(this->value);
	else return std::to_string(this->wideValue[0]) + std::to_string(this->wideValue[1]) + std::to_string(this->wideValue[2]) + std::to_string(this->wideValue[3]);
}