#include "Graph.hpp"

Graph::Edge::Edge(int from, int to, long long capacity):
	from(from),
	to(to),
	capacity(capacity)
{}

Graph::Graph():
	vertices(0)
{}

void Graph::addEdge(int from, int to, long long capacity) {
	edge.push_back(Edge(from, to, capacity));
	vertices = std::max(vertices, std::max(from + 1, to + 1));
}

void Graph::read() {
	int edges;
	std::cin >> vertices >> edges;
	for (int i = 0; i < edges; ++i) {
		int from, to, capacity;
		std::cin >> from >> to >> capacity;
		addEdge(from - 1, to - 1, capacity);
	}
}
