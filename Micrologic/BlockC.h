#pragma once

#include "Block.h"

class BlockC :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};
