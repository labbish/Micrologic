#include "BlockR.h"

namespace labbish {
	namespace Micrologic {
		bool BlockR::check() {
			for (Line* l : this->inputLines) if (l->mode != Line::LINE) return false;
			for (Line* l : this->outputLines) if (l->mode != Line::LINE) return false;
			return this->inputLines.size() == 2 && this->outputLines.size() == 1;
		}

		void BlockR::tick() {
			this->outputLines[0]->nextValue = this->inputLines[0]->value || this->inputLines[1]->value;
		}
	}
}