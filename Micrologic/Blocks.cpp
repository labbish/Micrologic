#include "Blocks.h"

void Blocks::add(std::vector<Line> L) {
	for (Line l : L) this->L.push_back(l);
}

void Blocks::add(std::vector<BlockN> N) {
	for (BlockN n : N) this->N.push_back(n);
}

void Blocks::add(std::vector<BlockA> A) {
	for (BlockA a : A) this->A.push_back(a);
}

void Blocks::add(std::vector<BlockR> R) {
	for (BlockR r : R) this->R.push_back(r);
}

void Blocks::add(std::vector<BlockT> T) {
	for (BlockT t : T) this->T.push_back(t);
}

void Blocks::tick() {
	for (int i = 0; i < N.size(); i++) N[i].tick();
	for (int i = 0; i < A.size(); i++) A[i].tick();
	for (int i = 0; i < R.size(); i++) R[i].tick();
	for (int i = 0; i < T.size(); i++) T[i].tick();
	for (int i = 0; i < L.size(); i++) L[i].flush();
}

void Blocks::clear() {
	std::vector<Line>().swap(L);
	std::vector<BlockN>().swap(N);
	std::vector<BlockA>().swap(A);
	std::vector<BlockR>().swap(R);
	std::vector<BlockT>().swap(T);
}