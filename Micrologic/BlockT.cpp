#include "BlockT.h"

namespace labbish {
	namespace Micrologic {
		bool BlockT::check() {
			return this->inputLines.size() == 1 && this->outputLines.size() == 1;
		}

		void BlockT::tick() {
			this->outputLines[0]->nextValue = this->inputLines[0]->value;
			for (int i = 0; i < 4; i++) this->outputLines[0]->nextWideValue[i] = this->inputLines[0]->wideValue[i];
		}
	}
}