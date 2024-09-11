#pragma once

#include "Block.h"

class BlockA :public Block {
public:
	using Block::Block;
	bool check();
	void tick();
};
