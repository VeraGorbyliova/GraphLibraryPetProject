#include "IncorporatedLogger.h"
#include "SparseGraph.h"
#include "DenseGraph.h"
#include "GraphPath.h"
#include "GraphIterator.h"
#include "Edge.h"
#include "Exception.h"
#include "WeightedGraph.h"
#include "ShortPathSearch.h"

#include <iostream>
#include <vector>

struct Road {
    Road(int c1, int c2, int l)
        : crs_1(c1), crs_2(c2), length_(std::round(l)) {}
    int crs_1, crs_2;
    double length_;
};

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

    WeightedSparseGraph g(crossroads.size(), true);
    for (const auto& r : roads) {
        g.insert(r.crs_1, r.crs_2, r.length_);
    }
    int simpsons_house = 10;
    int moes_bar = 32;
    std::cout << g << '\n';
    DijkstraPath path(g, simpsons_house);
    std::cout << "\n/////////RESULT//////////" << std::endl;
    path.printSolution();
}
int main()
{
    GomerPath();
    return 0;
}