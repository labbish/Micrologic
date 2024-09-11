#pragma once

#include "Block.h"

class BlockN :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};