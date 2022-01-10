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
class FordBellmanSearch {
	Graph g_;
	int n, m, vertex;
	bool negativeCycle = false;
	std::vector<std::shared_ptr<WeightedEdge>> e;
	std::vector<double> distanse;
public:
	FordBellmanSearch(const Graph& g, int vertex) : g_(g), vertex(vertex) {
		n = g_.vertexCount();
		m = g_.edgeCount();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (g_.getEdge(i, j).get() != nullptr)
					e.push_back(g_.getEdge(i, j));
			}
		}
		std::vector<double> DistanseArr(n, INT_MAX);
		DistanseArr[vertex] = 0;
		for (int i = 0; i < n - 1; ++i) {
			for (int j = 0; j < m; ++j) {
				if (DistanseArr[(*e[j]).v()] != INT_MAX && DistanseArr[(*e[j]).v()] + (*e[j]).wt() < DistanseArr[(*e[j]).w()])
					DistanseArr[(*e[j]).w()] = DistanseArr[(*e[j]).v()] + (*e[j]).wt();
			}
		}
		for (int i = 0; i < m; i++) {
			int x = (*e[i]).v();
			int y = (*e[i]).w();
			int weight = (*e[i]).wt();
			if (DistanseArr[x] != INT_MAX && DistanseArr[x] + weight < DistanseArr[y]) {
				negativeCycle = true;
			}
		}
		distanse = DistanseArr;
	}
	void PrintVxDistFromSrc() {
		std::cout << "Vertex \t Distance from Source" << std::endl;
		for (int i = 0; i < distanse.size(); i++)
			std::cout << i << "\t\t" << distanse[i] << std::endl;
	}
	std::vector<std::shared_ptr<WeightedEdge>> ExistEdges() { return e; }
	bool negCycleExist() { return negativeCycle; }
};
#endif

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
			//auto neg = FordBellmanSearch(g, vertex);
		    //neg.negCycleExist() == false
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
			std::cout << "Vertex \t Distance from Source\tPath" << std::endl;
			for (int i = 0; i < V; i++) {
				if (d[i] == INT_MAX) { 
					std::cout <<vertex<<"->"<< i << " \t\t" << "Infinite";
				}
				else 
					std::cout << vertex << "->" << i << " \t\t" << d[i] ;
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
			std::cout << "\t\t" << i;
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
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
void GomerPath() {
	std::vector<int> crossroads(36);
	std::generate(std::begin(crossroads), std::end(crossroads), []() {
		static int count;
		return count++;
	});

	std::vector<Road> roads;
	int k = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			roads.push_back(Road(k, k + 1, fRand(1.0, 10.0)));
			roads.push_back(Road(k + 1, k, fRand(1.0, 10.0)));

			roads.push_back(Road(k, k + 6, fRand(1.0, 10.0)));
			roads.push_back(Road(k + 6, k, fRand(1.0, 10.0)));

			k++;
		}
		roads.push_back(Road(k, k + 6, fRand(1.0, 10.0)));
		roads.push_back(Road(k + 6, k, fRand(1.0, 10.0)));

		k++;
	}
	for (int i = 0; i < 5; i++) {
		roads.push_back(Road(k, k + 1, fRand(1.0, 10.0)));
		roads.push_back(Road(k + 1, k, fRand(1.0, 10.0)));

		k++;
	}

	WeightedSparseGraph g(crossroads.size() + 2, true);
	for (const auto& r : roads) {
		g.insert(r.crs_1, r.crs_2, r.length_);
	}
	int simpsons_house = 10;
	int moes_bar = 32;
	std::cout << g << '\n';
	DijkstraPath path(g, simpsons_house);
	std::cout << "\n/////////Length//////////" << std::endl;
	path.printSolution();
	std::cout << "\n/////////Path//////////" << std::endl;
}