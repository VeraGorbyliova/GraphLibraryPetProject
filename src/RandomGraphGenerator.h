#pragma once
#include <iostream>
#include <type_traits>

#include "Edge.h"
#include "SparseGraph.h"
#include "DenseGraph.h"
#include "WeightedGraph.h"
template<typename GraphType>
class RandomGraph
{
	GraphType& g_;
	int edge;
	bool direction;
	int count = 0;
public:
	RandomGraph(GraphType& g, int e) :g_(g), edge(e){
		double p = 2.0 * edge / g_.vertexCount() / (g_.vertexCount() - 1);
		if constexpr (std::is_same_v<DenseGraph, std::decay_t<GraphType>> || std::is_same_v<WeightedDenseGraph, std::decay_t<GraphType>>) {
			for (int i = 0; i < g_.vertexCount(); i++) {
				for (int j = 0; j < i; j++) {
					if (rand() < p * RAND_MAX){
						if constexpr  (std::is_same_v<WeightedDenseGraph, std::decay_t<GraphType>>) {
							double weight = double(rand()/ (1.0 + RAND_MAX));
							g_.insert(i, j, weight);
						}
						else {
							g_.insert(Edge(i, j));
						}
					}
					
				}
			}
		}
		if constexpr (std::is_same_v<SparseGraph, std::decay_t<GraphType>> || std::is_same_v<WeightedSparseGraph, std::decay_t<GraphType>>) {
			for (int i = 0; i < edge; i++) { 
				int v = int(g_.vertexCount() * rand()/ (1.0 + RAND_MAX));
				int w = int(g_.vertexCount() * rand()/ (1.0 + RAND_MAX));
				if constexpr  (std::is_same_v<WeightedSparseGraph, std::decay_t<GraphType>>) {
					double weight = double(rand() / (1.0 + RAND_MAX));
					g_.insert(v, w, weight);
				}
				else {
					g_.insert(Edge(v, w));
				}
			}
		}		
	}
	friend class WeightedDenseGraph;
	friend class WeightedSparseGraph;
	friend class DenseGraph;
	friend class SparseGraph;
};