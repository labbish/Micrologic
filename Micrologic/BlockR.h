#pragma once

#include "Block.h"

class BlockR :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};
