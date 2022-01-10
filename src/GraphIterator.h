#pragma once

#include "Edge.h"
#include "Exception.h"
#include "IncorporatedLogger.h"

template <typename GraphType, typename IteratorType>
class GraphIterator {
	GraphType& g_;
	int vertex_number_;
	int current_pos_;
	IteratorType it_;

public:
	explicit GraphIterator(GraphType& g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{
		try {
			if (vertex >= g_.vertexCount() || vertex < 0 )
				throw std::logic_error("Vertex is out of range in Iterator");
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	/*explicit GraphIterator(GraphType g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{

	}*/

	auto begin() {
		it_ = g_.adj_[vertex_number_].begin();
		return it_;
	}

	auto next() {
		it_ = std::next(it_);
		return it_;
	}

	bool end() {
		if (it_ == g_.adj_[vertex_number_].end() ||
			*it_ == -1)
			return true;
		return false;
	}
};

template <typename GraphType, typename IteratorType, typename EdgeType=Edge>
class ConstGraphIterator {
	const GraphType& g_;
	int vertex_number_;
	int current_pos_;
	IteratorType it_;

public:
	explicit ConstGraphIterator(const GraphType& g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{
		try {
			if (vertex >= g_.vertexCount() || vertex < 0)
				throw std::logic_error("Vertex is out of range in Iterator");
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	auto begin() {
		it_ = g_.adj_[vertex_number_].cbegin();
		return it_;
	}

	auto next() {
		it_ = std::next(it_);
		return it_;
	}

	bool end() {
		if constexpr (std::is_same_v<EdgeType, WeightedEdge>) {
			if (it_ == g_.adj_[vertex_number_].cend()) {
				return true;
			}
			return false;
		}
		if constexpr (std::is_same_v<EdgeType, Edge>) {
			if (it_ == g_.adj_[vertex_number_].cend() || *it_ == -1)
				return true;
			return false;
		}

		return false;
	}
};

template <typename GraphType>
class ConstGraphIterator<GraphType, std::vector<std::shared_ptr<WeightedEdge>>::const_iterator, WeightedEdge> {
	const GraphType& g_;
	int vertex_number_;
	int current_pos_;
	std::vector<std::shared_ptr<WeightedEdge>>::const_iterator it_;

	std::vector<std::shared_ptr<WeightedEdge>> tmp_;

public:
	explicit ConstGraphIterator(const GraphType& g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{
		try {
			if (vertex >= g_.vertexCount() || vertex < 0)
				throw std::logic_error("Vertex is out of range in Iterator");
			std::copy_if(std::cbegin(g_.adj_[vertex_number_]), std::cend(g_.adj_[vertex_number_]),
				std::back_inserter(tmp_), [](const auto& v) { return v != nullptr; });
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	auto begin() {
		it_ = tmp_.cbegin();
		return it_;
	}

	auto next() {
		it_ = std::next(it_);
		return it_;
	}

	bool end() {
		if (it_ == tmp_.cend())
			return true;

		return false;
	}
};

template <typename GraphType,
	typename IteratorType = std::vector<int>::iterator>
class DenseGraphIterator {
	GraphType& g_;
	int vertex_number_;
	int current_pos_;
	IteratorType it_;

public:
	explicit DenseGraphIterator(GraphType& g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{
		try {
			if (vertex >= g_.vertexCount() || vertex < 0)
				throw std::logic_error("Vertex is out of range in Iterator");
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	auto begin() {
		it_ = g_.adj_[vertex_number_].begin();
		return it_;
	}

	auto next() {
		it_ = std::next(it_);
		return it_;
	}

	bool end() {
		if (it_ == g_.adj_[vertex_number_].end() ||
			*it_ == -1)
			return true;
		return false;
	}
};

template <typename GraphType, typename IteratorType=std::vector<int>::const_iterator, typename EdgeType=Edge>
class ConstDenseGraphIterator {
	const GraphType& g_;
	int vertex_number_;
	int current_pos_;
	IteratorType it_;

	std::vector<int> tmp_;
public:
	explicit ConstDenseGraphIterator(const GraphType& g, int vertex)
		: g_(g), vertex_number_(vertex), current_pos_(0)
	{
		try {
			if (vertex >= g_.vertexCount() || vertex < 0)
				throw std::logic_error("Vertex is out of range in Iterator");
			const auto& vec_it = g_.adj_[vertex_number_];
			for (auto begin_it = vec_it.begin(); begin_it != vec_it.end(); begin_it++) {
				if (*begin_it) {
					auto d = std::distance(vec_it.begin(), begin_it);
					tmp_.push_back(d);
				}
			}
		}
		catch (std::logic_error& exp) {
			LOG(WARNING, exp.what());
			throw GraphException(exp.what(), GraphException::GRAPH_VERTEX_RANGE_ERROR);
		}
	}

	auto begin() {
		it_ = tmp_.begin();
		return it_;
	}

	auto next() {
		it_ = std::next(it_);
		return it_;
	}

	bool end() {
		if (it_ == tmp_.end() || *it_ == -1)
			return true;
		return false;
	}
};
