#pragma once

#include "Block.h"

class BlockT :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};