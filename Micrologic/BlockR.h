#pragma once

#include "Block.h"

namespace labbish::Micrologic {
	class BlockR :public Block {
	public:
		using Block::Block;
		bool check();
		void tick();
	};
}
