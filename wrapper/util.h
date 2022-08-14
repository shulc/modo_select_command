#pragma once

#include <algorithm>

template <typename TContainer, typename TElement>
bool IsIn(TContainer& containter, const TElement& element) {
	return std::find(containter.begin(), containter.end(), element) != containter.end();
}