#ifndef NODE_HPP
#define NODE_HPP

#include <vector>
#include "edge.hpp"
#include "recorder.hpp"

class Edge;

class Node {
public:
	RecordNode nodeName;
	bool isFinalState;
	std::vector<Edge*> edges;

public:
	Node();
	~Node();
};


#endif