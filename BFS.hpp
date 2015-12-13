#ifndef __BFS__
#define __BFS__

#include "Network.hpp"
#include <vector>
#include <queue>

class BFS {
public:
	Network *network;
	std::vector <int> level;
	std::vector <bool> visited;

	void clear();

	void run();

	BFS(Network *network);

	bool isPathFound() const;

	bool betweenLevels(int from, int to) const;

	bool usefulNode(int node) const;
};

#endif
