#pragma once

#include <string>
#include <map>
#include <set>
#include <format>
#include <optional>
#include <algorithm>
#include "BlockN.h"
#include "BlockA.h"
#include "BlockR.h"
#include "BlockT.h"
#include "BlockC.h"
#include "BlockP.h"
#include "StableVector.h"

namespace labbish::Micrologic {
	using int_ = std::optional<int>;

	inline std::string to_string(int_ value) {
		std::string ans;
		if (value.has_value()) ans = std::to_string(*value);
		else ans = "\033[31m?\033[0m";
		return ans;
	}

	template <class T>
	inline void clearContainer(T& container) {
		T().swap(container);
	}

	class Blocks :public Block {
	public:
		std::string type;
		Blocks(std::string type = "");
		Blocks(const Blocks&);
		Blocks& operator=(const Blocks& other);
		Blocks(Blocks&&) noexcept;
		Blocks& operator=(Blocks&& other) noexcept;
		int speed = 1;
		std::vector<int> inputs;
		std::vector<int> outputs;
		std::map<std::string, std::string> mods; //modName->fileName
		StableVector<Line> L;
		StableVector<BlockN> N;
		StableVector<BlockA> A;
		StableVector<BlockR> R;
		StableVector<BlockT> T;
		StableVector<BlockC> C;
		StableVector<BlockP> P;
		StableVector<Blocks> Bs;
		int_ findLine(Line*)const;
		void addInput(std::vector<int> inputs);
		void addOutput(std::vector<int> outputs);
		void input(int order, bool value);
		void input(int order, std::array<bool, 4> value);
		std::string output(int order);
		std::vector<std::string> output();
		bool isInput(int line);
		bool isOutput(int line);
		void add(Line L);
		void add(BlockN N);
		void add(BlockA A);
		void add(BlockR R);
		void add(BlockT T);
		void add(BlockC C);
		void add(BlockP P);
		void add(const Blocks& Bs);
		void add(Blocks&& Bs);
		void tick();
		void clear();
		bool check();
	};
}