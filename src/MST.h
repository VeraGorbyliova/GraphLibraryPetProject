#pragma once

#include <vector>
#include <memory>

template <typename Graph, typename Edge>
class MST {
	const Graph& g_;
	std::vector<double> wt_;
	std::vector<std::shared_ptr<Edge>> fr_, mst_;

public:
	explicit MST(const Graph& g)
		: g_(g), mst_(g.vertexCount()), wt_(g.vertexCount(),
			g.vertexCount()), fr_(g.vertexCount())
	{
		int min = -1;
		for (int v = 0; min != 0; v = min) {
			min = 0;
			for (int w = 1; w < g_.vertexCount(); w++) {
				if (mst_[w] == nullptr) {
					if (auto e = g_.getEdge(v, w); e != nullptr) {
						if (auto p = e->wt(); p < wt_[w]) {
							wt_[w] = p;
							fr_[w] = e;
						}
					}
					if (wt_[w] < wt_[min]) {
						min = w;
					}
				}
			}
			if (min != 0) {
				mst_[min] = fr_[min];
			}
		}
		
	}

	void show() {
		for (int v = 1; v < g_.vertexCount(); v++) {
			if (mst_[v] != nullptr) {
				mst_[v]->show();
				std::cout << '\n';
			}
		}
	}
};


