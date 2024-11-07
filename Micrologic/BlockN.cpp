#include "BlockN.h"

namespace labbish {
	namespace Micrologic {
		bool BlockN::check() {
			for (Line* l : this->inputLines) if (l->mode != Line::LINE) return false;
			for (Line* l : this->outputLines) if (l->mode != Line::LINE) return false;
			return this->inputLines.size() == 1 && this->outputLines.size() == 1;
		}

		void BlockN::tick() {
			this->outputLines[0]->nextValue = !this->inputLines[0]->value;
		}
	}
}