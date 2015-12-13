#ifndef __PUSHRELABLEFLOWWORKER__
#define __PUSHRELABLEFLOWWORKER__

#include "FlowWorker.hpp"
#include <vector>

class PushRelabelFlowWorker : public FlowWorker {
public:
	std::vector <int> height;
	std::vector <long long> extra;
	std::vector <int> pointer;

	void push(int from, int to, long long delta);

	bool canPush(int from, int to);

	void initPreflow();

	bool overflowed(int node);

	PushRelabelFlowWorker(Network *network);

	void lift(int node);

	void discharge(int node);

	void run();
};

#endif
