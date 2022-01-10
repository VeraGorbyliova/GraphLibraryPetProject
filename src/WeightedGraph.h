#pragma once

#include <memory>
#include <vector>
#include <list>
#include <iostream>

#include "GraphIterator.h"
#include "Edge.h"
#include "MST.h"
#include "IncorporatedLogger.h"
#include "Exception.h"


class WeightedDenseGraph {
	using Container = std::vector<std::shared_ptr<WeightedEdge>>;

	int vertex_count_, edge_count_;
	bool is_directed_;
	std::vector<Container> adj_;
public:
	explicit WeightedDenseGraph(int n, bool is_directed)
		: vertex_count_(n), edge_count_(0), is_directed_(is_directed)
	{
		adj_.resize(vertex_count_);
		for (int i = 0; i < vertex_count_; i++) {
			adj_[i].assign(vertex_count_, nullptr);
		}
	}
	~WeightedDenseGraph() = default;
	inline int vertexCount() const noexcept { return vertex_count_; }
	inline int edgeCount() const noexcept { return edge_count_; };
	inline bool isDirected() const noexcept { return is_directed_; }
	void insert(std::shared_ptr<WeightedEdge> e) {
		try {
			if (e->v() >= vertex_count_ || e->v() < 0 || e->w() >= vertex_count_ || e->w() < 0)
				throw std::logic_error("Edge is out of range in Weighted Dense Graph");
			if (adj_[e->v()][e->w()] == nullptr) {
				edge_count_++;
			}
			adj_[e->v()][e->w()] = e;
			if (!is_directed_) {
				adj_[e->w()][e->v()] = e;
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
		
	}
	void insert(int v, int w, double wt) {
		try {
			if (v >= vertex_count_ || v < 0 || w >= vertex_count_ || w < 0) 
				throw std::logic_error("Vertex is out of range in Weighted Dense Graph");

			if (adj_[v][w] == nullptr) {
				edge_count_++;
			}
			auto e = std::make_shared<WeightedEdge>(v, w, wt);
			adj_[e->v()][e->w()] = e;
			if (!is_directed_) {
				adj_[e->w()][e->v()] = e;
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
		
	}
	int remove(std::shared_ptr<WeightedEdge> e) {
		try {
			if (e->v() >= vertex_count_ || e->v() < 0 || e->w() >= vertex_count_ || e->w() < 0)
				throw std::logic_error("Edge is out of range in Weighted Dense Graph");
			if (adj_[e->v()][e->w()] == nullptr) {
				edge_count_--;
				adj_[e->v()][e->w()].reset();
			}

			if (!is_directed_) {
				adj_[e->w()][e->v()].reset();
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	inline std::shared_ptr<WeightedEdge> getEdge(int v, int w) const {
		try { 
			if (v >= vertex_count_ || v < 0 || w >= vertex_count_ || w < 0)
				throw std::logic_error("Vertex is out of range in Weighted Dense Graph");
			return adj_[v][w]; 
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	bool hasEdge(int v, int w) const {
		if (getEdge(v, w) != nullptr) return true;
		else false;
	}

	using Iterator = GraphIterator<WeightedDenseGraph, Container::iterator>;
	using ConstIterator = ConstGraphIterator<WeightedDenseGraph, Container::const_iterator, WeightedEdge>;

	friend class GraphIterator<WeightedDenseGraph, typename Container::iterator>;
	friend class ConstGraphIterator<WeightedDenseGraph, typename Container::const_iterator, WeightedEdge>;
};

class WeightedSparseGraph {
	using Container = std::list<std::shared_ptr<WeightedEdge>>;

	int vertex_count_, edge_count_;
	bool is_directed_;
	std::vector<Container> adj_;

public:
	explicit WeightedSparseGraph(int n, bool is_directed)
		: vertex_count_(n), edge_count_(0), is_directed_(is_directed)
	{
		adj_.assign(vertex_count_, std::list<std::shared_ptr<WeightedEdge>>());
	}
	~WeightedSparseGraph() = default;

	inline int vertexCount() const noexcept { return vertex_count_; }
	inline int edgeCount() const noexcept { return edge_count_; }
	inline bool isDirected() const noexcept { return is_directed_; }

	void insert(std::shared_ptr<WeightedEdge> e) noexcept {
		try {
			if (e->v() >= vertex_count_ || e->v() < 0 || e->w() >= vertex_count_ || e->w() < 0)
				throw std::logic_error("Edge is out of range in Weighted Sparse Graph");
			if (getEdge(e->v(), e->w()) != nullptr) {
				return;
			}
			edge_count_++;
			adj_[e->v()].push_back(e);
			if (!is_directed_) {
				adj_[e->w()].push_back(e);
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	void insert(int v, int w, double wt) noexcept {
		try {
			if (v >= vertex_count_ || v < 0 || w >= vertex_count_ || w < 0)
				throw std::logic_error("Edge is out of range in Weighted Sparse Graph");
			if (getEdge(v, w) != nullptr) {
				return;
			}
			edge_count_++; //For Igor
			auto e = std::make_shared<WeightedEdge>(v, w, wt);
			adj_[e->v()].push_back(e);
			if (!is_directed_) {
				adj_[e->w()].push_back(e);
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	int remove(std::shared_ptr<WeightedEdge> e) noexcept {

	}
	inline std::shared_ptr<WeightedEdge> getEdge(int v, int w) const {
		try {
			if (v >= vertex_count_ || v < 0 || w >= vertex_count_ || w < 0)
				throw std::logic_error("Vertex is out of range in Weighted Sparse Graph");

			const auto& list_for_v = adj_[v];
			if (list_for_v.empty())
				return nullptr;
			const auto it = std::find_if(std::cbegin(list_for_v), std::cend(list_for_v),
				[v, w](const auto& e) {
					return (v == e->v()) && (w == e->w());
				});
			if (it == std::cend(list_for_v))
				return nullptr;
			return *it;
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	bool hasEdge(int v, int w) const {

			if (getEdge(v, w) != nullptr) return true;
			else false;

	}
	using Iterator = GraphIterator<WeightedSparseGraph, Container::iterator>;
	using ConstIterator = ConstGraphIterator<WeightedSparseGraph, Container::const_iterator, WeightedEdge>;

	friend class GraphIterator<WeightedSparseGraph, typename Container::iterator>;
	friend class ConstGraphIterator<WeightedSparseGraph, typename Container::const_iterator, WeightedEdge>;

};

std::ostream& operator<<(std::ostream& os, const WeightedDenseGraph g) {
	for (int s = 0; s < g.vertexCount(); s++) {
		os.width(2);
		os << s << ":";
		auto it = WeightedDenseGraph::ConstIterator(g, s);
		for (auto i = it.begin(); !it.end(); i = it.next()) {
			os.width(2);
			os << (*i)->other(s) << " ";
		}
		os << std::endl;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const WeightedSparseGraph g) {
	for (int s = 0; s < g.vertexCount(); s++) {
		os.width(2);
		os << s << ":";
		auto it = WeightedSparseGraph::ConstIterator(g, s);
		for (auto i = it.begin(); !it.end(); i = it.next()) {
			os.width(2);
			os << (*i)->other(s) << " ";
		}
		os << std::endl;
	}

	return os;
}


