#pragma once

#include "BlockN.h"
#include "BlockA.h"
#include "BlockR.h"
#include "BlockT.h"

class Blocks {
public:
	std::vector<Line> L;
	std::vector<BlockN> N;
	std::vector<BlockA> A;
	std::vector<BlockR> R;
	std::vector<BlockT> T;
	void add(std::vector<Line> L);
	void add(std::vector<BlockN> N);
	void add(std::vector<BlockA> A);
	void add(std::vector<BlockR> R);
	void add(std::vector<BlockT> T);
	void tick();
	void clear();
};
