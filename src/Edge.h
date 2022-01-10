#pragma once

struct Edge {
	int v, w;
	explicit Edge(int v = -1, int w = -1) : v(v), w(w) {}
};

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class WeightedEdge {
	int v_, w_;
	double weight_;

public:
	explicit WeightedEdge(int v, int w, double weight)
		: v_(v), w_(w), weight_(weight)
	{

	}

	inline int v() const noexcept { return v_; }
	inline int w() const noexcept { return w_; }
	inline double wt() const noexcept { return weight_; }
	bool from(int v) const noexcept;
	int other(int i) const noexcept {
		return v_ == i ? w_ : v_;
	}

	void show() {
		std::cout << v_ << " - " << w_;
	}
};