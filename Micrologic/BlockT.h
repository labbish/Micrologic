#pragma once

#include "Block.h"

namespace labbish {
	namespace Micrologic {
		class BlockT :public Block {
		public:
			using Block::Block;
			bool check();
			void tick();
		};
	}
}