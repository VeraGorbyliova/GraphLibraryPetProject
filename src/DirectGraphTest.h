#pragma once

#include "SparseGraph.h"
#include "DenseGraph.h"
#include "GraphIterator.h"
#include "IncorporatedLogger.h"
#include "Exception.h"

template <typename GraphIn, typename GraphOut>
void reverse(const GraphIn& in, GraphOut& out) {
	for (int v = 0; v < in.vertexCount(); v++) {
		auto it = GraphIn::ConstIteratorType(in, v);
		for (auto w = it.begin(); !it.end(); w = it.next()) {
			out.insert(Edge(*w, v));
		}
	}
}

template <typename TCGraph, typename Graph>
class TC {
	TCGraph tc_;
public:
	explicit TC(const Graph& g) : tc_(g) {
		for (int i = 0; i < tc_.vertexCount(); i++) {
			tc_.insert(Edge(i, i));
		}
		for (int i = 0; i < tc_.vertexCount(); i++) {
			for (int j = 0; j < tc_.vertexCount(); j++) {
				if (tc_.hasEdge(j, i)) {
					for (int t = 0; t < tc_.vertexCount(); t++) {
						if (tc_.hasEdge(i, t)) {
							tc_.insert(Edge(j, t));
						}
					}
				}
			}
		}
	}

	bool reachable(int v, int w) const noexcept {
		return tc_.hasEdge(v, w);
	}
};

template <typename DAG>
class TS {
	const DAG& g_;
	int count_, tcount_;
	std::vector<int> pre_, post_, postI_;

	void tsR_rev(int v) {
		pre_[v] = count_++;
		auto it = DAG::ConstIteratorType(g_, v);
		for (auto t = it.begin(); !it.end(); t = it.next()) {
			if (pre_[*t] == -1) {
				tsR_rev(*t);
			}
		}
		post_[v] = tcount_;
		postI_[tcount_++] = v;
	}

	void tsR(int v) {
		pre_[v] = count_++;
		for (int w = 0; w < g_.vertexCount(); w++) {
			if (g_.hasEdge(w, v)) {
				if (pre_[w] == -1) {
					tsR(w);
				}
			}
		}
		post_[v] = tcount_;
		postI_[tcount_] = v;
	}
public:
	TS(const DAG& g, bool reverse = false) : g_(g), tcount_(0), count_(0),
		pre_(g_.vertexCount(), -1),
		post_(g_.vertexCount(), -1),
		postI_(g_.vertexCount(), -1)
	{
		for (int v = 0; v < g_.vertexCount(); v++) {
			if (pre_[v] == -1) {
				if (reverse) {
					tsR_rev(v);
				}
				else {
					tsR(v);
				}
			}
		}
	}

	int operator[](int v) const {
		return postI_[v];
	}

	int relabel(int v) const {
		return post_[v];
	}
};

void directGraph_Test() {
	auto g =DenseGraph(13, true);
	g.insert(Edge(0, 2));
	g.insert(Edge(1, 0));
	g.insert(Edge(2, 3));
	g.insert(Edge(2, 4));
	g.insert(Edge(3, 2));
	g.insert(Edge(3, 4));
	g.insert(Edge(4, 5));
	g.insert(Edge(4, 6));
	g.insert(Edge(5, 3));
	g.insert(Edge(5, 0));
	g.insert(Edge(6, 0));
	g.insert(Edge(6, 7));
	g.insert(Edge(7, 8));
	g.insert(Edge(8, 7));
	g.insert(Edge(9, 6));
	g.insert(Edge(9, 8));
	g.insert(Edge(9, 12));
	g.insert(Edge(10, 9));
	g.insert(Edge(11, 4));
	g.insert(Edge(11, 9));
	g.insert(Edge(12, 11));
	g.insert(Edge(12, 10));

	std::cout << g << '\n';
	auto g2 = DenseGraph(13, true);
	//reverse(g, g2);
	//std::cout << g2 << '\n';

	auto ts = TS<DenseGraph>(g);
	std::cout << ts[0];

}