#ifndef __MKMFlowWorker__
#define __MKMFlowWorker__

#include "FlowWorker.hpp"
#include <vector>
#include <queue>

class MKMFlowWorker : public FlowWorker {
public:
	struct BFS {
		Network * network;
		std::vector <int> level;
		std::vector <bool> visited;

		void clear();

		void run();

		BFS(Network * network);

		bool isPathFound() const;

		bool betweenLevels(int from, int to) const;

		bool usefulNode(int node) const;
	};

	BFS bfs;
	std::vector <long long> outCapacity;
	std::vector <long long> inCapacity;
	std::vector <bool> usedNode;
	std::vector <int> pointer[2];

	MKMFlowWorker(Network * network);

	long long getPotential(int node);

	void pushPreflow(int node, long long flow, bool direction);

	void initBlockingFlow();

	void deleteNode(int node);

	void findBlockingFlow();

	void run();
};

#endif
