#pragma once

#include <iostream>
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <algorithm>

#include "GraphPath.h"
#include "GraphIterator.h"
#include "Edge.h"
#include "RandomGraphGenerator.h"
#include "VertexDegree.h"
#include "Exception.h"

#if 1
class DenseGraph {
	int vertex_number_;
	int edge_number_;
	bool directed_;
public:
	std::vector<std::vector<bool>> adj_;
	DenseGraph(int v_n, bool has_direct = false) : vertex_number_(v_n), edge_number_(0), directed_(has_direct)
	{
		adj_.resize(vertex_number_);
		for (int i = 0; i < vertex_number_; i++) {
			adj_[i].assign(vertex_number_, false);
		}
	}
	DenseGraph() { }
	~DenseGraph() {	}
	int vertexCount() const noexcept { return vertex_number_; }
	int edgeCount() const noexcept { return edge_number_; }
	bool isDirected() const noexcept { return directed_; }
	void insert(const Edge& e) {
		try {
			if (e.v >= vertex_number_ || e.v < 0 || e.w >= vertex_number_ || e.w < 0)
				throw std::logic_error("Edge is out of range in Dense Graph");
			if (adj_[e.v][e.w] == false) edge_number_++;
			adj_[e.v][e.w] = true;
			if (!directed_) adj_[e.w][e.v] = true;
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	void remove(const Edge& e) {
		try {
			if (e.v >= vertex_number_ || e.v < 0 || e.w >= vertex_number_ || e.w < 0)
				throw std::logic_error("Edge is out of range in Dense Graph");
			if (adj_[e.v][e.w] == true) edge_number_--;
			adj_[e.v][e.w] = false;
			if (!directed_) adj_[e.w][e.v] = false;
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::EDGE_RANGE_ERROR);
		}
	}
	bool hasEdge(int v, int w) const  {
		try {
			if (v >= vertex_number_ || v < 0 || w >= vertex_number_ || w < 0)
				throw std::logic_error("Vertex is out of range in Dense Graph");
			return adj_[v][w];
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}			
	}
	void Print() {
		for (int i = 0; i < vertexCount(); i++) {
			for (int j = 0; j < vertexCount(); j++) {
				std::cout << adj_[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
	using IteratorType = DenseGraphIterator<DenseGraph>;
	using ConstIteratorType = ConstDenseGraphIterator<DenseGraph>;
	friend class DenseGraphIterator<DenseGraph>;
	friend class ConstDenseGraphIterator<DenseGraph>;
};
std::ostream& operator<<(std::ostream& os, const DenseGraph& g) {
	for (int s = 0; s < g.vertexCount(); s++) {
		os.width(2);
		os << s << ":";
		auto it = DenseGraph::ConstIteratorType(g, s);
		int i = 0;
		for (auto t = it.begin(); !it.end(); t = it.next()) {
			os.width(2);
			if (*t != 0) os << i << " ";
			i++;
		}
		os << std::endl;
	}
	return os;
}
#endif