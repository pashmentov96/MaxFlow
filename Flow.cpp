#include <iostream>
#include <vector>
#include <queue>

class Graph {
public:
	struct Edge {
		int from, to;
		long long capacity;
 
		Edge(int from, int to, long long capacity):
			from(from),
			to(to),
			capacity(capacity)
		{}
	};

	std::vector <Edge> edge;
	int vertices;

	Graph():
		vertices(0)
	{}

	void addEdge(int from, int to, long long capacity) {
		edge.push_back(Edge(from, to, capacity));
		vertices = std::max(vertices, std::max(from + 1, to + 1));
	}

	void read() {
		int edges;
		std::cin >> vertices >> edges;
		for (int edge = 0; edge < edges; ++edge) {
			int from, to, capacity;
			std::cin >> from >> to >> capacity;
			addEdge(from - 1, to - 1, capacity);
		}
	}
};

class Network {
public:
	struct Arc {
		long long flow, capacity;

		Arc(): 
			flow(0),
			capacity(0)
		{}

		bool isSaturated() const {
			return flow == capacity;
		}

		long long getRemainingCapacity() {
			return capacity - flow;
		}
	};

	std::vector <std::vector <Arc> > arc;
	int nodes, source, sink;
	long long totalFlow;

	void addEdge(Graph::Edge edge) {
		arc[edge.from][edge.to].capacity += edge.capacity;
	}

	Network(const Graph &graph):
		arc(graph.vertices, std::vector <Arc>(graph.vertices)),
		nodes(graph.vertices),
		source(0), sink(graph.vertices - 1),
		totalFlow(0)
	{
		for (size_t edgeId = 0; edgeId < graph.edge.size(); ++edgeId) {
			addEdge(graph.edge[edgeId]);
		}
	}

	void push(int from, int to, long long delta) {
		arc[from][to].flow += delta;
		arc[to][from].flow -= delta;
	}
};

class FlowWorker {
public:
	Network * network;
	long long totalFlow;

	FlowWorker(Network * network):
		network(network),
		totalFlow(0)
	{}

	virtual void run() = 0;
};

class MKMFlowWorker: public FlowWorker {
public:
	struct BFS {
		Network * network;
		std::vector <int> level;
		std::vector <bool> visited;

		void clear() {
			fill(visited.begin(), visited.end(), false);
		}

		void run() {
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

		BFS(Network * network):
			network(network),
			level(network->nodes),
			visited(network->nodes)
		{}

		bool isPathFound() const {
			return visited[network->sink];
		}

		bool betweenLevels(int from, int to) const  {
			return level[from] + 1 == level[to];
		}

		bool usefulNode(int node) const {
			if (!visited[node]) {
				return false;
			}
			return node == network->sink || level[node] < level[network->sink];
		}
	};

	BFS bfs;
	std::vector <long long> outCapacity, inCapacity;
	std::vector <bool> usedNode;
	std::vector <int> pointer[2];

	MKMFlowWorker(Network * network):
		FlowWorker(network),
		bfs(network),
		outCapacity(network->nodes),
		inCapacity(network->nodes),
		usedNode(network->nodes)
	{
		pointer[0] = pointer[1] = std::vector <int>(network->nodes);
	}

	long long getPotential(int node) {
		if (node == network->source) {
			return outCapacity[node];
		} else if (node == network->sink) {
			return inCapacity[node];
		} else {
			return std::min(inCapacity[node], outCapacity[node]);
		}
	}

	void pushPreflow(int node, long long flow, bool direction) {
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

	void initBlockingFlow() {
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

	void deleteNode(size_t node) {
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

	void findBlockingFlow() {
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

	virtual void run() {
		while (true) {
			bfs.run();
			if (!bfs.isPathFound()) {
				break;
			}
			findBlockingFlow();
		}
	}
};

class PushRelabelFlowWorker: public FlowWorker {
public:
	std::vector <int> height;
	std::vector <long long> extra;
	std::vector <int> pointer;

	void push(int from, int to, long long delta) {
		extra[to] += delta;
		extra[from] -= delta;
		network->push(from, to, delta);
	}

	bool canPush(int from, int to) {
		return height[from] == height[to] + 1;
	}

	void initPreflow() {
		height[network->source] = network->nodes;
		for (int node = 0; node < network->nodes; ++node) {
			long long delta = network->arc[network->source][node].getRemainingCapacity();
			push(network->source, node, delta);
		}
	}

	bool overflowed(int node) {
		return node != network->sink && node != network->source && extra[node] > 0;
	}

	PushRelabelFlowWorker(Network *network):
		FlowWorker(network),
		height(network->nodes),
		extra(network->nodes),
		pointer(network->nodes)
	{}

	void lift(int node) {
		int newHeight = network->nodes * 2;
		for (int to = 0; to < network->nodes; ++to) {
			if (network->arc[node][to].isSaturated()) {
				continue;
			}
			newHeight = std::min(newHeight, height[to] + 1);
		}
		height[node] = newHeight;
	}

	void discharge(int node) {
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

	virtual void run() {
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
};

int main() {
	Graph graph;
	graph.read();
	Network network(graph);
	//FlowWorker * FlowWorker = new PushRelabelFlowWorker(&network);
	//FlowWorker * FlowWorker = new MKMFlowWorker(&network);
	FlowWorker->run();
	std::cout << FlowWorker->totalFlow << '\n';
	for (size_t edgeId = 0; edgeId < graph.edge.size(); ++edgeId) {
		Graph::Edge edge = graph.edge[edgeId];
		long long flow = network.arc[edge.from][edge.to].flow;
		long long result = std::max(0LL, std::min(flow, edge.capacity));
		network.push(edge.to, edge.from, result);
		std::cout << result << '\n';
	}
	return 0;
}
