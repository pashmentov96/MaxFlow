#ifndef __NETWORK__
#define __NETWORK__

#include <vector>
#include "Graph.hpp"

class Network {
public:
	struct Arc {
		long long flow;
		long long capacity;

		Arc(): 
			flow(0),
			capacity(0)
		{}

		bool isSaturated() const {
			return flow == capacity;
		}

		long long getRemainingCapacity() {
			return capacity - flow;
		}
	};

	std::vector <std::vector <Arc> > arc;
	int nodes;
	int source;
	int sink;
	long long totalFlow;

	void addEdge(Graph::Edge edge) {
		arc[edge.from][edge.to].capacity += edge.capacity;
	}

	Network(const Graph &graph):
		arc(graph.vertices, std::vector <Arc>(graph.vertices)),
		nodes(graph.vertices),
		source(0), sink(graph.vertices - 1),
		totalFlow(0)
	{
		for (int edgeId = 0; edgeId < graph.edge.size(); ++edgeId) {
			addEdge(graph.edge[edgeId]);
		}
	}

	void push(int from, int to, long long delta) {
		arc[from][to].flow += delta;
		arc[to][from].flow -= delta;
	}
};

#endif
