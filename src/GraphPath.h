#pragma once
#include "IncorporatedLogger.h"
#include "Exception.h"
#include "WeightedGraph.h"

#include <vector>
#include <list>
#include <stack>
#include <iostream>
#include <typeinfo>
#include <stdexcept>
#include <algorithm>
#include <optional>
#include <cassert>
#include <set>

template <typename GraphType, typename IteratorType>
class GraphDegree {
	const GraphType& g;
	std::vector<int> degree;

public:
	GraphDegree(GraphType& g) : g(g), degree(g.vertexCount(), 0) {
		/*if constexpr (std::is_const_v<GraphType> || std::is_reference_v<GraphType>) {
			GraphType = std::decay_t<GraphType>;
		}*/

		for (int v = 0; v < g.vertexCount(); v++) {
			IteratorType it(g, v);
			for (auto w = it.begin(); !it.end(); w = it.next()) {
				degree[v]++;
			}
		}
	}

	int operator[](int v) const { 
		try {
			if (v < 0 && v >= g.vertexCount())
				throw std::logic_error("Wrong vertex number. Vertex does not exist in graph.");
			return degree[v];
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
};

template <typename GraphType, typename Iterator>
class SimplePath {
	GraphType& g_;
	std::vector<bool> visited_;
	std::list<int> vertex_list_;
	bool found;

	bool searchR(int v, int w) {
		if (v == w) return true;

		visited_[v] = true;
		Iterator it(g_, v);

		for (auto i = it.begin(); !it.end(); i = it.next()) {
			if (!visited_[*i]) {
				if (searchR(*i, w)) {
					vertex_list_.push_back(*i);
					return true;
				}
			}
		}
		return false;
	}

public:
	explicit SimplePath(GraphType& g, int v, int w)
		: g_(g), visited_(g_.vertexCount(), false), found(false)
	{
		try {
			if (v >= g_.vertexCount() || v < 0 || w >= g_.vertexCount() || w < 0)
				throw std::logic_error("Vertex is out of range in Simple Path");
			found = searchR(v, w);
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}		
	}

	bool exist() const noexcept { return found; }
	const auto& getVertexList() const noexcept {
		return vertex_list_;
	}
};

template <typename GraphType, typename Iterator>
class GamiltonPath {
	GraphType& g_;
	std::vector<bool> visited_;
	std::list<int> vertex_list_;
	bool found;

	bool searchR(int v, int w, int d) {
		if (v == w) return (d == 0);

		visited_[v] = true;
		Iterator it(g_, v);

		for (auto i = it.begin(); !it.end(); i = it.next()) {
			if (!visited_[*i]) {
				if (searchR(*i, w, d - 1)) {
					vertex_list_.push_back(*i);
					return true;
				}
			}
		}
		visited_[v] = false;
		return false;
	}

public:
	explicit GamiltonPath(GraphType& g, int v, int w, int d = -1)
		: g_(g), visited_(g_.vertexCount(), false), found(false)
	{
		try {
			if (v >= g_.vertexCount() || v < 0 || w >= g_.vertexCount() || w < 0)
				throw std::logic_error("Vertex is out of range in Gamilton Path");
			if (d == -1)
				found = searchR(v, w, g_.vertexCount() - 1);
			else
				found = searchR(v, w, d);
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
	bool exist() const noexcept { return found; }
	const auto& getVertexList() const noexcept {
		return vertex_list_;
	}
};

template <typename GraphType, typename Iterator>
class EulerPath {
	GraphType g_;
	int v, w;
	std::stack<int> vertex_stack_;
	bool found;

	int tour(int v) noexcept {
		while (true) {
			Iterator it(g_, v);
			auto w = it.begin();
			if (it.end())
				break;

			const auto w_val = *w;
			vertex_stack_.push(v);
			g_.remove(v, w_val);
			v = w_val;
		}
		return v;
	}
public:
	explicit EulerPath(const GraphType& g, int v, int w)
		: g_(g), v(v), w(w), found(false)
	{
		try {
			if (v >= g_.vertexCount() || v < 0 || w >= g_.vertexCount() || w < 0)
				throw std::logic_error("Vertex is out of range in Euler Path");
			GraphDegree<GraphType, Iterator> deg(g_);
			auto t = deg[v] + deg[w];
			if ((t % 2) != 0) {
				found = false;
				return;
			}
			for (t = 0; t < g_.vertexCount(); t++) {
				if (t != v && t != w) {
					if ((deg[t] % 2) != 0) {
						found = false;
						return;
					}
				}
			}
			found = true;
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
	bool exist() const noexcept { return found; }
	/*const auto& getVertexList() const noexcept {
		return vertex_list_;
	}*/
	void show() noexcept {
		if (!found) return;
		while (tour(v) == v && !vertex_stack_.empty()) {
			v = vertex_stack_.top();
			vertex_stack_.pop();
			std::cout << "-" << v;
		}
		std::cout << '\n';
	}
};