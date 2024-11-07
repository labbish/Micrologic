#pragma once

#include <string>
#include <map>
#include <set>
#include <format>
#include "BlockN.h"
#include "BlockA.h"
#include "BlockR.h"
#include "BlockT.h"
#include "BlockC.h"
#include "BlockP.h"
#include "StableVector.h"

namespace labbish {
	namespace Micrologic {
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
			int findLine(Line*);
			void addInput(std::vector<int> inputs);
			void addOutput(std::vector<int> outputs);
			void input(int order, bool value);
			void input(int order, std::array<bool, 4> value);
			std::string output(int order);
			std::vector<std::string> output();
			bool isInput(int line);
			bool isOutput(int line);
			void add(std::vector<Line> L);
			void add(std::vector<BlockN> N);
			void add(std::vector<BlockA> A);
			void add(std::vector<BlockR> R);
			void add(std::vector<BlockT> T);
			void add(std::vector<BlockC> C);
			void add(std::vector<BlockP> P);
			void add(const Blocks& Bs);
			void add(Blocks&& Bs);
			void add(std::vector<Blocks> Bs);
			void tick();
			void clear();
			bool check();
			std::vector<std::string> exportBlocks();
		};
	}
}