#pragma once
#include <vector>
#include <map>
#include <iostream>
#include <algorithm> 

#include "Edge.h"
#include "WeightedGraph.h"
#include "IncorporatedLogger.h"
#include "Exception.h"

#if 1
template<typename Graph>
class DijkstraPath {
	Graph g_;
	std::vector<std::shared_ptr<WeightedEdge>> e;
	int V, vertex;
	std::vector<double> d;
	std::vector <double> path;
	bool negative = false;
	std::vector<double> parent;
public:
	DijkstraPath(Graph g, int src) : g_(g), vertex(src), V(g_.vertexCount()) {
		try {
			if (src >= g_.vertexCount()|| src < 0 )
				throw std::logic_error("Vertex is out of range in Dijkstra's Algorithm");
			if (true) {
				std::vector<double> dist(V, INT_MAX);
				std::vector<bool> sptSet(V, false);
				std::vector<double> par(V, -1);
				dist[vertex] = 0;
				for (int count = 0; count < V - 1; count++) {
					auto u = std::round(minDistance(dist, sptSet));
					sptSet[u] = true;
					for (int v = 0; v < V; v++)
						if (g_.getEdge(u, v) != nullptr) {
							if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + (*(g_.getEdge(u, v))).wt() < dist[v]) {
								dist[v] = dist[u] + (*(g_.getEdge(u, v))).wt();
								par[v] = u;
							}	
						}
				}
				d = dist;
				parent = par;
			}
			else {
				std::cout << "Contains negative cycle. You can't use Dijkstra's Algorithm" << std::endl;
				negative = true;
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}
	void printSolution()
	{
		if(negative == false){
			std::cout << "==== WAY =========== DISTANCE ========== PATH ====" << std::endl;
			for (int i = 0; i < V; i++) {
				if (d[i] == INT_MAX) { 
					std::cout.width(5);
					std::cout << vertex << " -> " << i << "\t\t" << 0 << "\tInfinite";
				}
				else {
					std::cout.width(5);
					std::cout << vertex << " -> " << i << "\t\t" << d[i] << "\t\t" << vertex;
				}
				printShortPath(i);
				std::cout << std::endl;
			}
		}
		else 
			std::cout << "No solution" << std::endl;
	}
	void printShortPath(int i) {
			if (parent[i] == -1)
				return;
			printShortPath(parent[i]);
			std::cout << " -> " << i;
	}
private:
	double minDistance(std::vector<double> dist, std::vector<bool> sptSet) {
		double min = INT_MAX, min_index;
		for (int v = 0; v < V; v++) {
			if (sptSet[v] == false && dist[v] <= min)
				min = dist[v], min_index = v;
		}
		return min_index;
	}
};
#endif
