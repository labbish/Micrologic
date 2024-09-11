#pragma once

#include <string>
#include <map>
#include "BlockN.h"
#include "BlockA.h"
#include "BlockR.h"
#include "BlockT.h"

class Blocks :public Block {
public:
	std::vector<int> inputs;
	std::vector<int> outputs;
	std::map<std::string, std::string> mods; //modName->fileName
	std::vector<Line> L;
	std::vector<BlockN> N;
	std::vector<BlockA> A;
	std::vector<BlockR> R;
	std::vector<BlockT> T;
	std::vector<Blocks> Bs;
	void addInput(std::vector<int> inputs);
	void addOutput(std::vector<int> outputs);
	void input(int order, bool value);
	bool output(int order);
	std::vector<bool> output();
	void add(std::vector<Line> L);
	void add(std::vector<BlockN> N);
	void add(std::vector<BlockA> A);
	void add(std::vector<BlockR> R);
	void add(std::vector<BlockT> T);
	void add(std::vector<Blocks> Bs);
	void tick();
	void clear();
	bool check();
};
