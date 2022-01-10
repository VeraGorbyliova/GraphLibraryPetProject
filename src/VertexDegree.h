#pragma once
#include <vector>
#include <typeinfo>
#include <stdexcept>
#include <list>
#include <type_traits>
#include <cstdint>

#include "GraphIterator.h"
#include "SparseGraph.h"
#include "DenseGraph.h"
#include "WeightedGraph.h"
#if 1
template<typename GraphType>
class Degree
{
	const GraphType& g_;
	std::vector<int> v_degree;
	int vertex;
public:
	Degree(GraphType& g) : g_(g) {
		v_degree.resize(g_.vertexCount());
		for (int v = 0; v < g_.vertexCount(); v++) {
			auto it = GraphType::ConstIterator(g, v);
			for (auto t = it.begin(); !it.end(); t = it.next()) {
				if constexpr (std::is_same_v<WeightedDenseGraph, std::decay_t<GraphType>> || std::is_same_v<WeightedSparseGraph, std::decay_t<GraphType>>) {
					if (*t != nullptr) v_degree[v]++; 
				}
				if constexpr (std::is_same_v<DenseGraph, std::decay_t<GraphType>>) { 
					if (*t != 0) v_degree[v]++; 
				}
				if constexpr (std::is_same_v<SparseGraph, std::decay_t<GraphType>>) {
					v_degree[v]++;
				}
			}
		}
	}
	int operator[] (int v) const {
		try { 
			if(v<0 && v >= g_.vertexCount())
				throw std::logic_error("Vertex is out of range in Vertex Degree.");
			return v_degree[v]; 
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}	
	}
};
#endif