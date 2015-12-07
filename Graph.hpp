#ifndef __GRAPH__
#define __GRAPH__

#include <vector>
#include <algorithm>
#include <iostream>

class Graph {
public:
	struct Edge {
		int from;
		int to;
		long long capacity;
 
		Edge(int from, int to, long long capacity);
	};

	std::vector <Edge> edge;
	int vertices;

	Graph();

	void addEdge(int from, int to, long long capacity);

	void read();
};

#endif
