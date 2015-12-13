#include "Graph.hpp"
#include "Network.hpp"
#include "PushRelabelFlowWorker.hpp"
#include "MKMFlowWorker.hpp"
#include <iostream>
#include <vector>
#include <queue>

int main() {
	Graph graph;
	graph.read();
	Network network(graph);
	FlowWorker * FlowWorker = new MKMFlowWorker(&network);
	//FlowWorker * FlowWorker = new PushRelabelFlowWorker(&network);
	FlowWorker->run();
	std::cout << FlowWorker->totalFlow << '\n';
	for (int edgeId = 0; edgeId < graph.edge.size(); ++edgeId) {
		Graph::Edge edge = graph.edge[edgeId];
		long long flow = network.arc[edge.from][edge.to].flow;
		long long result = std::max(0LL, std::min(flow, edge.capacity));
		network.push(edge.to, edge.from, result);
		std::cout << result << '\n';
	}
	return 0;
}
