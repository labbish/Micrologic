#include "Blocks.h"

namespace labbish::Micrologic {
	Blocks::Blocks(std::string type) :type(type) {
	}

	Blocks::Blocks(const Blocks& other) {
		type = other.type;
		speed = other.speed;
		inputs = other.inputs;
		outputs = other.outputs;
		mods = other.mods;
		L = other.L;
		N = other.N;
		A = other.A;
		R = other.R;
		T = other.T;
		C = other.C;
		P = other.P;
		Bs = other.Bs;
	}

	Blocks& Blocks::operator=(const Blocks& other) {
		type = other.type;
		speed = other.speed;
		inputs = other.inputs;
		outputs = other.outputs;
		mods = other.mods;
		L = other.L;
		N = other.N;
		A = other.A;
		R = other.R;
		T = other.T;
		C = other.C;
		P = other.P;
		Bs = other.Bs;
		return *this;
	}

	Blocks::Blocks(Blocks&& other) noexcept {
		type = other.type;
		speed = other.speed;
		inputs = std::move(other.inputs);
		outputs = std::move(other.outputs);
		mods = std::move(other.mods);
		L = std::move(other.L);
		N = std::move(other.N);
		A = std::move(other.A);
		R = std::move(other.R);
		T = std::move(other.T);
		C = std::move(other.C);
		P = std::move(other.P);
		Bs = std::move(other.Bs);
		other.speed = 0;
	}

	Blocks& Blocks::operator=(Blocks&& other) noexcept {
		if (this != &other) {
			inputs.clear();
			outputs.clear();
			mods.clear();
			L.clear();
			N.clear();
			A.clear();
			R.clear();
			T.clear();
			C.clear();
			P.clear();
			Bs.clear();
			type = other.type;
			speed = other.speed;
			inputs = std::move(other.inputs);
			outputs = std::move(other.outputs);
			mods = std::move(other.mods);
			L = std::move(other.L);
			N = std::move(other.N);
			A = std::move(other.A);
			R = std::move(other.R);
			T = std::move(other.T);
			C = std::move(other.C);
			P = std::move(other.P);
			Bs = std::move(other.Bs);
			other.speed = 0;
		}
		return *this;
	}

	int_ Blocks::findLine(Line* l) const {
		for (int i = 0; i < L.size(); i++) {
			if (l == &(L[i])) return i;
		}
		return std::nullopt;
	}

	void Blocks::add(Line l) {
		this->L.push_back(l);
	}

	void Blocks::add(BlockN n) {
		this->N.push_back(n);
	}

	void Blocks::add(BlockA a) {
		this->A.push_back(a);
	}

	void Blocks::add(BlockR r) {
		this->R.push_back(r);
	}

	void Blocks::add(BlockT t) {
		this->T.push_back(t);
	}

	void Blocks::add(BlockC c) {
		this->C.push_back(c);
	}

	void Blocks::add(BlockP p) {
		this->P.push_back(p);
	}

	void Blocks::add(const Blocks& b) {
		this->Bs.push_back(b);
	}

	void Blocks::add(Blocks&& b) {
		this->Bs.push_back(std::move(b));
	}

	void Blocks::tick() {
		for (int i = 0; i < speed; i++) {
			for (int i = 0; i < inputLines.size(); i++) {
				L[inputs[i]].nextValue = L[inputs[i]].value = inputLines[i]->value;
				for (int j = 0; j < 4; j++) L[inputs[i]].nextWideValue[j] = L[inputs[i]].wideValue[j] = inputLines[i]->wideValue[j];
			}
			for (BlockN& n : N) n.tick();
			for (BlockA& a : A) a.tick();
			for (BlockR& r : R) r.tick();
			for (BlockT& t : T) t.tick();
			for (BlockC& c : C) c.tick();
			for (BlockP& p : P) p.tick();
			for (Blocks& b : Bs) b.tick();
			for (Line& l : L) l.flush();
			for (int i = 0; i < outputLines.size(); i++) {
				outputLines[i]->nextValue = outputLines[i]->value = L[outputs[i]].value;
				for (int j = 0; j < 4; j++) outputLines[i]->nextWideValue[j] = outputLines[i]->wideValue[j] = L[outputs[i]].wideValue[j];
			}
		}
	}

	void Blocks::clear() {
		clearContainer(L);
		clearContainer(N);
		clearContainer(A);
		clearContainer(R);
		clearContainer(T);
		clearContainer(C);
		clearContainer(P);
		clearContainer(Bs);
		clearContainer(inputs);
		clearContainer(outputs);
		clearContainer(mods);
	}

	void Blocks::addInput(std::vector<int> inputs) {
		for (int i : inputs) this->inputs.push_back(i);
	}

	void Blocks::addOutput(std::vector<int> outputs) {
		for (int i : outputs) this->outputs.push_back(i);
	}

	void Blocks::input(int order, bool value) {
		this->L[inputs[order]].value = this->L[inputs[order]].nextValue = value;
		for (int i = 0; i < 4; i++) this->L[inputs[order]].wideValue[i] = this->L[inputs[order]].nextWideValue[i] = value;
	}

	void Blocks::input(int order, std::array<bool, 4> value) {
		for (int i = 0; i < 4; i++) this->L[inputs[order]].wideValue[i] = this->L[inputs[order]].nextWideValue[i] = value[i];
	}

	std::string Blocks::output(int order) {
		return this->L[outputs[order]].checkValue();
	}

	std::vector<std::string> Blocks::output() {
		std::vector<std::string> outs;
		for (int i : outputs) {
			outs.push_back(L[i].checkValue());
		}
		return outs;
	}

	bool Blocks::isInput(int line) {
		return std::ranges::contains(inputs, line);
	}

	bool Blocks::isOutput(int line) {
		return std::ranges::contains(outputs, line);
	}

	bool Blocks::check() {
		if (!(this->inputs.size() == this->inputLines.size() && this->outputs.size() == this->outputLines.size())) return false;
		for (int i = 0; i < this->inputs.size(); i++) if (this->L[inputs[i]].mode != this->inputLines[i]->mode) return false;
		for (int i = 0; i < this->outputs.size(); i++) if (this->L[outputs[i]].mode != this->outputLines[i]->mode) return false;
		return true;
	}
}