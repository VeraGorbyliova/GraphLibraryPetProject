#pragma once
#include <stdexcept>
#include <windows.h>

#if 1
class GraphException{
	//const std::string* file;
	int exp;
	std::string msg;
	const std::string* func;
	const std::string* info;
public:
	GraphException() {}
	GraphException(std::string && msg) : msg(msg) {}
	GraphException(std::string && msg, int exp) :msg(msg), exp(exp) {}
	GraphException(std::string && msg, const std::string* file_, int exp, const std::string* func_, const std::string* info_) : msg(msg),
		//file(file_),
		exp(exp),
		func(func_),
		info(info_)
	{
	}
	enum EXEPTION {
		OUT_OF_RANGE = -1, 
		RUNTIME_ERROR = -2,
		MULTITHREADING_ERROR = -3,
		GRAPH_VERTEX_RANGE_ERROR = -4,
		EDGE_RANGE_ERROR = -5,
		PATH_ERROR_ISOLATED_VERTEX = -6,
		ITERATOR_ERROR = -7,
	};
	~GraphException() {}
	const std::string& what() const noexcept { return msg; }
	//const std::string get_file() const { return file; }
	int get_line() const { return exp; }
	const std::string* get_func() const { return func; }
	const std::string* get_info() const { return info; }
};
#endif
