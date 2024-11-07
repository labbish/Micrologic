#include "Block.h"

namespace labbish {
	namespace Micrologic {
		Block::Block() {
			this->inputLines = {};
		}

		Block::Block(std::vector<Line*> inputLines, std::vector<Line*> outputLines) {
			this->inputLines = inputLines;
			this->outputLines = outputLines;
		}

		Block::Block(const Block& other) {
			inputLines = other.inputLines;
			outputLines = other.outputLines;
		}

		Block& Block::operator=(const Block& other) noexcept {
			inputLines = other.inputLines;
			outputLines = other.outputLines;
			return *this;
		}

		Block::Block(Block&& other) noexcept {
			inputLines = std::move(other.inputLines);
			outputLines = std::move(other.outputLines);
			other.inputLines.clear();
			other.outputLines.clear();
		}

		Block& Block::operator=(Block&& other) noexcept {
			if (this != &other) {
				inputLines.clear();
				outputLines.clear();
				inputLines = std::move(other.inputLines);
				outputLines = std::move(other.outputLines);
				other.inputLines.clear();
				other.outputLines.clear();
			}
			return *this;
		}
	}
}