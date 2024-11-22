#include "Line.h"

namespace labbish {
	namespace Micrologic {
		Line::Line(TYPE mode) {
			this->mode = mode;
			this->value = this->nextValue = 0;
			for (int i = 0; i < 4; i++) this->wideValue[i] = this->nextWideValue[i] = 0;
		}

		Line::Line(const Line& other) {
			mode = other.mode;
			value = other.value;
			nextValue = other.nextValue;
			wideValue = other.wideValue;
			nextWideValue = other.nextWideValue;
		}

		Line& Line::operator=(const Line& other) {
			mode = other.mode;
			value = other.value;
			nextValue = other.nextValue;
			wideValue = other.wideValue;
			nextWideValue = other.nextWideValue;
			return *this;
		}

		Line::Line(Line&& other) noexcept {
			mode = other.mode;
			value = other.value;
			nextValue = other.nextValue;
			wideValue = other.wideValue;
			nextWideValue = other.nextWideValue;
			other.mode = LINE;
			other.value = 0;
			other.nextValue = 0;
			other.wideValue.fill(0);
			other.nextWideValue.fill(0);
		}

		Line& Line::operator=(Line&& other) noexcept {
			if (this != &other) {
				mode = other.mode;
				value = other.value;
				nextValue = other.nextValue;
				wideValue = other.wideValue;
				nextWideValue = other.nextWideValue;
				other.mode = LINE;
				other.value = 0;
				other.nextValue = 0;
				other.wideValue.fill(0);
				other.nextWideValue.fill(0);
			}
			return *this;
		}

		void Line::set(bool value) {
			this->value = this->nextValue = value;
		}

		void Line::set(std::array<bool, 4> wideValue) {
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
	}
}