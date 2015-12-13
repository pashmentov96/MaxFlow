#include "BFS.hpp"

void BFS::clear() {
	fill(visited.begin(), visited.end(), false);
}

void BFS::run() {
	clear();
	std::queue <int> queue;
	queue.push(network->source);
	level[network->source] = 0;
	visited[network->source] = true;
	while (!queue.empty()) {
		int node = queue.front();
		queue.pop();
		for (int to = 0; to < network->nodes; ++to) {
			if (visited[to]) {
				continue;
			}
			const Network::Arc &arc = network->arc[node][to];
			if (arc.isSaturated()) {
				continue;
			}
			level[to] = level[node] + 1;
			visited[to] = true;
			queue.push(to);
		}
	}
}

BFS::BFS(Network *network):
	network(network),
	level(network->nodes),
	visited(network->nodes)
{}

bool BFS::isPathFound() const {
	return visited[network->sink];
}

bool BFS::betweenLevels(int from, int to) const {
	return level[from] + 1 == level[to];
}

bool BFS::usefulNode(int node) const {
	if (!visited[node]) {
		return false;
	}
	return node == network->sink || level[node] < level[network->sink];
}
