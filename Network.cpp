#include "Network.hpp"

Network::Arc::Arc(): 
	flow(0),
	capacity(0)
{}

bool Network::Arc::isSaturated() const {
	return flow == capacity;
}

long long Network::Arc::getRemainingCapacity() {
	return capacity - flow;
}

void Network::addEdge(Graph::Edge edge) {
	arc[edge.from][edge.to].capacity += edge.capacity;
}

Network::Network(const Graph &graph):
	arc(graph.vertices, std::vector <Arc>(graph.vertices)),
	nodes(graph.vertices),
	source(0), sink(graph.vertices - 1),
	totalFlow(0)
{
	for (size_t edgeId = 0; edgeId < graph.edge.size(); ++edgeId) {
		addEdge(graph.edge[edgeId]);
	}
}

void Network::push(int from, int to, long long delta) {
	arc[from][to].flow += delta;
	arc[to][from].flow -= delta;
}
