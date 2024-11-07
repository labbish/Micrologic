#pragma once

#include <vector>
#include "Line.h"

namespace labbish {
	namespace Micrologic {
		class Block {
		public:
			std::vector<Line*> inputLines;
			std::vector<Line*> outputLines;
			Block();
			Block(std::vector<Line*> inputLines, std::vector<Line*> outputLines);
			Block(const Block&);
			Block& operator=(const Block&) noexcept;
			Block(Block&&) noexcept;
			Block& operator=(Block&&) noexcept;
			virtual bool check() = 0;
			virtual void tick() = 0;
		};
	}
}