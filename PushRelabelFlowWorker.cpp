#include "PushRelabelFlowWorker.hpp"

void PushRelabelFlowWorker::push(int from, int to, long long delta) {
	extra[to] += delta;
	extra[from] -= delta;
	network->push(from, to, delta);
}

bool PushRelabelFlowWorker::canPush(int from, int to) {
	return height[from] == height[to] + 1;
}

void PushRelabelFlowWorker::initPreflow() {
	height[network->source] = network->nodes;
	for (int node = 0; node < network->nodes; ++node) {
		long long delta = network->arc[network->source][node].getRemainingCapacity();
		push(network->source, node, delta);
	}
}

bool PushRelabelFlowWorker::overflowed(int node) {
	return node != network->sink && node != network->source && extra[node] > 0;
}

PushRelabelFlowWorker::PushRelabelFlowWorker(Network * network):
	FlowWorker(network),
	height(network->nodes),
	extra(network->nodes),
	pointer(network->nodes)
{}

void PushRelabelFlowWorker::lift(int node) {
	int newHeight = network->nodes * 2;
	for (int to = 0; to < network->nodes; ++to) {
		if (network->arc[node][to].isSaturated()) {
			continue;
		}
		newHeight = std::min(newHeight, height[to] + 1);
	}
	height[node] = newHeight;
}

void PushRelabelFlowWorker::discharge(int node) {
	while (extra[node] > 0) {
		int &to = pointer[node];
		if (to == network->nodes) {
			lift(node);
			to = 0;
			continue;
		}
		if (canPush(node, to)) {
			long long delta = std::min(extra[node], network->arc[node][to].getRemainingCapacity());
			push(node, to, delta);
		}
		++to;
	}
}

void PushRelabelFlowWorker::run() {
	initPreflow();
	bool changed = true;
	while (changed) {
		changed = false;
		for (int node = 0; node < network->nodes; ++node) {
			if (overflowed(node)) {
				changed = true;
				discharge(node);
			}
		}
	}
	for (int node = 0; node < network->nodes; ++node) {
		if (node != network->source) {
			totalFlow += network->arc[network->source][node].flow;
		}
	}
}
