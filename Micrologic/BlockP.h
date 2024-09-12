#pragma once

#include "Block.h"

class BlockP :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};
