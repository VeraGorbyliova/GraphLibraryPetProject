#pragma once

#include <vector>
#include <queue>
#include <iostream>

#include "Edge.h"
#include "GraphIterator.h"
#include "Exception.h"
#include "IncorporatedLogger.h"

template <class Graph>
class Search {
protected:
	const Graph& g_;
	int count_;
	std::vector<int> ord_;

	virtual void searchC(Edge) = 0;
	void search() {
		for (int v = 0; v < g_.vertexCount(); v++) {
			if (ord_[v] == -1) {
				searchC(Edge(v, v));
			}
		}
	}

public:
	Search(const Graph& g) : g_(g), ord_(g_.vertexCount(), -1), count_(0) {
	}

	int operator[] (int v) const {
		try {
			if (v < 0 && v >= g_.vertexCount())
				throw std::logic_error("Vertex is out of range in Search.");
			return ord_[v];
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}	
	}
};

template <typename Graph>
class BFS : public Search<Graph> {
	std::vector<int> st_;

	void searchC(Edge e) {
		std::queue<Edge> q;
		q.push(e);
		while (!q.empty()) {
			e = q.front();
			q.pop();
			if (this->ord_[e.w] == -1) {
				auto v = e.v, w = e.w;
				this->ord_[w] = this->count_++;
				st_[w] = v;
				auto it = Graph::ConstIteratorType(this->g_, w);
				for (auto t = it.begin(); !it.end(); t = it.next()) {
					if (this->ord_[*t] == -1)
						q.push(Edge(w, *t));
				}
			}
		}
	}

public:
	BFS(Graph& g) : Search<Graph>(g), st_(g.vertexCount(), -1) {
		this->search();
	}
	int ST(int v) const { 
		try {
			if (v < 0 && v >= g_.vertexCount())
				throw std::logic_error("Vertex is out of range in Breadth First Search.");
			return st_[v];
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
};

template <typename Graph>
void search_Test() {
	//D<int> d;
	//d.f();
	Graph g(8);
	g.insert(Edge(0, 2));
	g.insert(Edge(0, 5));
	g.insert(Edge(0, 7));
	g.insert(Edge(2, 6));
	g.insert(Edge(5, 4));
	g.insert(Edge(5, 3));
	g.insert(Edge(7, 1));
	g.insert(Edge(7, 4));
	g.insert(Edge(6, 4));
	g.insert(Edge(7, 4));
	g.insert(Edge(3, 4));


	std::cout << g << "\n\n\n";
	auto s = BFS<Graph>(g);

	std::cout << s.ST(0);
}