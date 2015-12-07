#ifndef __NETWORK__
#define __NETWORK__

#include <vector>
#include "Graph.hpp"

class Network {
public:
	struct Arc {
		long long flow;
		long long capacity;

		Arc();

		bool isSaturated() const;

		long long getRemainingCapacity();
	};

	std::vector <std::vector <Arc> > arc;
	int nodes;
	int source;
	int sink;
	long long totalFlow;

	void addEdge(Graph::Edge edge);

	Network(const Graph &graph);

	void push(int from, int to, long long delta);
};

#endif
