#pragma once

#include <queue>

template<class P, class T>
using MinQueue = std::priority_queue<std::tuple<P, T>,
                                     std::vector<std::tuple<P, T>>,
                                     std::greater<std::tuple<P, T>>>;

template<class P, class T, class It>
MinQueue<P, T> makeMinQueue(It begin, It end)
{
	std::vector<std::tuple<P, T>> elems;
	
	for (It it = begin; it != end; it++)
	{
		elems.emplace_back(it->second, it->first);
	}

	return MinQueue<P, T>(elems.begin(), elems.end());
}

template<class P, class T>
T popTop(MinQueue<P, T>& pqueue)
{
	T elem = std::get<1>(pqueue.top());

	pqueue.pop();

	return elem;
}
