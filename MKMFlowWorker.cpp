#include "MKMFlowWorker.hpp"

void MKMFlowWorker::BFS::clear() {
	fill(visited.begin(), visited.end(), false);
}

void MKMFlowWorker::BFS::run() {
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

MKMFlowWorker::BFS::BFS(Network * network):
	network(network),
	level(network->nodes),
	visited(network->nodes)
{}

bool MKMFlowWorker::BFS::isPathFound() const {
	return visited[network->sink];
}

bool MKMFlowWorker::BFS::betweenLevels(int from, int to) const  {
	return level[from] + 1 == level[to];
}

bool MKMFlowWorker::BFS::usefulNode(int node) const {
	if (!visited[node]) {
		return false;
	}
	return node == network->sink || level[node] < level[network->sink];
}

MKMFlowWorker::MKMFlowWorker(Network * network):
	FlowWorker(network),
	bfs(network),
	outCapacity(network->nodes),
	inCapacity(network->nodes),
	usedNode(network->nodes)
{
	pointer[0] = pointer[1] = std::vector <int>(network->nodes);
}

long long MKMFlowWorker::getPotential(int node) {
	if (node == network->source) {
		return outCapacity[node];
	} else if (node == network->sink) {
		return inCapacity[node];
	} else {
		return std::min(inCapacity[node], outCapacity[node]);
	}
}

void MKMFlowWorker::pushPreflow(int node, long long flow, bool direction) {
	if (flow == 0 || (direction == 0 && node == network->sink) || (direction == 1 && node == network->source)) {
		return ;
	}
	for (int &next = pointer[direction][node]; next < network->nodes; ++next) {
		int from = node, to = next;
		if (direction) {
			std::swap(from, to);
		}
		if (usedNode[next] || !bfs.betweenLevels(from, to) || network->arc[from][to].isSaturated()) {
			continue;
		}
		long long delta = std::min(flow, network->arc[from][to].getRemainingCapacity());
		outCapacity[from] -= delta;
		inCapacity[to] -= delta;
		network->push(from, to, delta);
		flow -= delta;
		pushPreflow(next, delta, direction);
		if (flow == 0) {
			break;
		}
	}
}

void MKMFlowWorker::initBlockingFlow() {
	fill(pointer[0].begin(), pointer[0].end(), 0);
	fill(pointer[1].begin(), pointer[1].end(), 0);
	fill(outCapacity.begin(), outCapacity.end(), 0);
	fill(inCapacity.begin(), inCapacity.end(), 0);

	for (int node = 0; node < network->nodes; ++node) {
		usedNode[node] = !bfs.usefulNode(node);
	}
	for (int node = 0; node < network->nodes; ++node) {
		if (usedNode[node]) {
			continue;
		}
		for (int to = 0; to < network->nodes; ++to) {
			if (bfs.betweenLevels(node, to) && !usedNode[to]) {
				long long capacity = network->arc[node][to].getRemainingCapacity();
				outCapacity[node] += capacity;
				inCapacity[to] += capacity;
			}
		}
	}
}

void MKMFlowWorker::deleteNode(int node) {
	usedNode[node] = true;
	for (int to = 0; to < network->nodes; ++to) {
		if (usedNode[to]) {
			continue;
		}
		if (bfs.betweenLevels(node, to)) {
			inCapacity[to] -= network->arc[node][to].getRemainingCapacity();
		}
		if (bfs.betweenLevels(to, node)) {
			outCapacity[to] -= network->arc[to][node].getRemainingCapacity();
		}
	}
}

void MKMFlowWorker::findBlockingFlow() {
	initBlockingFlow();
	while (true) {
		int node = network->nodes;
		for (int candidate = 0; candidate < network->nodes; ++candidate) {
			if (usedNode[candidate]) {
				continue;
			}
			if (node == network->nodes || getPotential(candidate) < getPotential(node)) {
				node = candidate;
			}
		}
		if (node == network->nodes) {
			break;
		}
		long long delta = getPotential(node);
		totalFlow += delta;
		pushPreflow(node, delta, 0);
		pushPreflow(node, delta, 1);
		deleteNode(node);
	}
}

void MKMFlowWorker::run() {
	while (true) {
		bfs.run();
		if (!bfs.isPathFound()) {
			break;
		}
		findBlockingFlow();
	}
}
