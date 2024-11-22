#include "BlockP.h"

namespace labbish {
	namespace Micrologic {
		bool BlockP::check() {
			for (Line* l : this->inputLines) if (l->mode != Line::WIDELINE) return false;
			for (Line* l : this->outputLines) if (l->mode != Line::LINE) return false;
			return this->inputLines.size() == 1 && this->outputLines.size() == 4;
		}

		void BlockP::tick() {
			for (int i = 0; i < 4; i++) {
				this->outputLines[i]->nextValue = this->inputLines[0]->wideValue[i];
			}
		}
	}
}