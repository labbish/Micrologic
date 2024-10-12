#pragma once

#include <forward_list>

template <typename T>
class StableVector {
public:
	std::forward_list<T> list;
};