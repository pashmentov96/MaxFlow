#ifndef __MKMFlowWorker__
#define __MKMFlowWorker__

#include "FlowWorker.hpp"
#include "BFS.hpp"
#include <vector>
#include <queue>

class MKMFlowWorker : public FlowWorker {
public:
	BFS bfs;
	std::vector <long long> outCapacity;
	std::vector <long long> inCapacity;
	std::vector <bool> usedNode;
	std::vector <int> pointer[2];

	MKMFlowWorker(Network *network);

	long long getPotential(int node);

	void pushPreflow(int node, long long flow, bool direction);

	void initBlockingFlow();

	void deleteNode(int node);

	void findBlockingFlow();

	void run();
};

#endif
