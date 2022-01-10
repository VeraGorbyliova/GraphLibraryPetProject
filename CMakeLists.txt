cmake_minimum_required(VERSION 2.8)
project(graphs)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(SOURCE_EXE main.cpp)

#set(HEADER_LIB edge.h graph.h DenseGraph.h sparsegraph.h GraphIterator.h graphpath.h)

#add_library(graph STATIC ${HEADER_LIB})

add_executable(main ${SOURCE_EXE})	
#target_link_libraries(main graph)	