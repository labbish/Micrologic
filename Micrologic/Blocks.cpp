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

void Blocks::add(std::vector<Blocks> Bs) {
	for (Blocks b : Bs) this->Bs.push_back(b);
}

void Blocks::tick() {
	//printf("S%d,%d\n", int(inputLines.size()), int(outputLines.size()));
	for (int i = 0; i < inputLines.size(); i++) {
		L[inputs[i]].nextValue = L[inputs[i]].value = inputLines[i]->value;
		//printf("I%d\n", L[inputs[i]].value);
	}
	for (int i = 0; i < N.size(); i++) N[i].tick();
	for (int i = 0; i < A.size(); i++) A[i].tick();
	for (int i = 0; i < R.size(); i++) R[i].tick();
	for (int i = 0; i < T.size(); i++) T[i].tick();
	for (int i = 0; i < Bs.size(); i++) Bs[i].tick();
	for (int i = 0; i < L.size(); i++) L[i].flush();
	for (int i = 0; i < outputLines.size(); i++) {
		outputLines[i]->nextValue = outputLines[i]->value = L[outputs[i]].value;
		//printf("O%d\n", outputLines[i]->value);
	}
	if (Bs.size() != 0) for (auto l : Bs[0].L) printf("%d ", l.value);
	printf("\n");
}

void Blocks::clear() {
	std::vector<Line>().swap(L);
	std::vector<BlockN>().swap(N);
	std::vector<BlockA>().swap(A);
	std::vector<BlockR>().swap(R);
	std::vector<BlockT>().swap(T);
}

void Blocks::addInput(std::vector<int> inputs) {
	for (int i : inputs) this->inputs.push_back(i);
}

void Blocks::addOutput(std::vector<int> outputs) {
	for (int i : outputs) this->outputs.push_back(i);
}

void Blocks::input(int order, bool value) {
	this->L[inputs[order]].value = this->L[inputs[order]].nextValue = value;
}

bool Blocks::output(int order) {
	return this->L[outputs[order]].value;
}

std::vector<bool> Blocks::output() {
	std::vector<bool> outs;
	for (int i : outputs) {
		outs.push_back(L[i].value);
	}
	return outs;
}

bool Blocks::check() {
	return this->inputs.size() == this->inputLines.size() && this->outputs.size() == this->outputLines.size();
}