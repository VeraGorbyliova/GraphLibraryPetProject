#pragma once
#include "GraphIterator.h"
#include "IncorporatedLogger.h"
#include "Exception.h"
#include <vector>
#include <list>
#include <iostream>
#include <queue>
#include <typeinfo>
#include <stack>

#if 1
template<typename GraphType> 
class DepthFirstSearch
{
	const GraphType& g_;
	std::vector<int> visited;
	std::stack<int> stk;
	int vertex;
public:
	DepthFirstSearch() {}
	DepthFirstSearch(int vertex, GraphType& g) : vertex(vertex), g_(g) {
		try {
			for (int i = 0; i < g_.vertexCount(); i++) {
				visited.push_back(0);
			}
			stk.push(0);
			search(vertex);
		}
		catch (std::out_of_range&) {
			
		}
	}
private:
	void search(int vertex) {	
		try {
			if (v >= g.vertexCount() || v < 0)
				throw std::logic_error("Vertex is out of range in Depth First Search");
			while (!stk.empty()) {
				vertex = stk.top();
				stk.pop();
				visited[vertex] = 1;
				std::cout << vertex << "\t";
				auto it = GraphType::ConstIteratorType(g_, vertex);
				for (auto i = it.begin(); !it.end(); i = it.next()) {
					if (visited[*i] == 0) {
						stk.push(*i);
						search(*i);
					}
				}
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
		
	}
	friend class DenseGraph;
	friend class SparseGraph;
};
#endif
#if 0
template<typename GraphType>
class BreadthFirstSearch
{
	const GraphType& g_;
	std::vector<int> visited;
	std::queue<int> queue;
	int vertex;
public:
	BreadthFirstSearch() {}
	BreadthFirstSearch(int vertex, GraphType& g) : vertex(vertex), g_(g) {
		for (int i = 0; i < g_.vertexCount(); i++) {
			visited.push_back(0);
		}
		queue.push(0);
		search(vertex);
	}
private:
	void search(int vertex) {
		while (!queue.empty()) {
			vertex = queue.front();
			queue.pop();
			visited[vertex] = 2;
			auto it = GraphType::ConstIteratorType(g_, vertex);
			for (auto i = it.begin(); !it.end(); i = it.next()) {
				if (g_.adj_[vertex][*i] == 0 && visited[*i] == 0) {
					queue.push(*i);
					std::cout << queue.back() << "\t";
					search(*i);
				}
			}
		}
	}
	friend class DenseGraph;
	friend class SparseGraph;
};
#endif
