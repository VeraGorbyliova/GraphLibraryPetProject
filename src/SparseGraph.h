#pragma once

#include <list>
#include <vector>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <cassert>
#include <set>

#include "Edge.h"
#include "GraphIterator.h"
#include "GraphPath.h"
#include "IncorporatedLogger.h"
#include "Exception.h"


class SparseGraph {
	std::vector<std::list<int>> adj_;
	std::size_t v_count_, e_count_;
	bool directed;

public:
	explicit SparseGraph(size_t v, size_t is_directed = false)
		: v_count_(v), e_count_(0), directed(is_directed)
	{
		adj_.assign(v_count_, std::list<int>());
	}

	~SparseGraph() {}

	int vertexCount() const noexcept { return v_count_; }
	int edgeCount() const noexcept { return e_count_; }
	bool isDirected() const noexcept { return directed; }

	void insert(const Edge& e) {
		try
		{
			if (e.v >= v_count_ || e.v < 0 || e.w >= v_count_ || e.w < 0)
				throw std::logic_error("Edge is out of range in Sparse Graph");
			auto& list = adj_.at(e.v);
			if (list.empty()) {
				list.push_back(e.w);
				if (!directed) {
					adj_[e.w].push_back(e.v);
				}

				return;
			}
			if (std::find(std::cbegin(list), std::cend(list), e.w)
				== std::cend(list)) {
				list.push_back(e.w);
				if (!directed) {
					adj_[e.w].push_back(e.v);
				}
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}

	void removeNode(int v) {
		try {
			if (v >= adj_.size()) {
				throw std::logic_error("Invoking not existing nod in Sparse Graph");
			}
			return;
			adj_[v].clear();
			adj_[v].push_back(-1);
			for (auto& vertex : adj_) {
				const auto adj_it = std::find(std::cbegin(vertex), std::cend(vertex), v);
				if (adj_it == std::cend(vertex)) {
					continue;
				}

				vertex.erase(adj_it);
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
	bool hasEdge(size_t v, size_t w) const {
		try {
			if (v >= v_count_ || v < 0 || w >= v_count_ || w < 0)
				throw std::logic_error("Vertex is out of range in  Sparse Graph");
			const auto& list = adj_.at(v);
			return std::find(std::cbegin(list), std::cend(list), w) != std::cend(list);
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
	void remove(size_t v, size_t w){
		try
		{
			if (v >= v_count_ || v < 0 || w >= v_count_ || w < 0)
				throw std::logic_error("Vertex is out of range in Sparse Graph");
			adj_.at(v).remove(w);
			if (!directed) {
				adj_.at(w).remove(v);
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	using IteratorType = GraphIterator<SparseGraph, std::list<int>::iterator>;
	using ConstIteratorType = ConstGraphIterator<SparseGraph, std::list<int>::const_iterator>;
	friend class GraphIterator<SparseGraph, typename std::list<int>::iterator>;
	friend class ConstGraphIterator<SparseGraph, typename std::list<int>::const_iterator>;

};

std::ostream& operator<<(std::ostream& os, const SparseGraph g) {
	for (int s = 0; s < g.vertexCount(); s++) {
		os.width(2);
		os << s << ":";
		auto it = SparseGraph::ConstIteratorType(g, s);
		for (auto i = it.begin(); !it.end(); i = it.next()) {
			os.width(2);
			os << *i << " ";
		}
		os << std::endl;
	}

	return os;
}

