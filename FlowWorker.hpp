#ifndef __FLOWWORKER__
#define __FLOWWORKER__

#include "Network.hpp"

class FlowWorker {
public:
	Network *network;
	long long totalFlow;

	FlowWorker(Network *network):
		network(network),
		totalFlow(0)
	{}

	virtual void run() = 0;
};

#endif
