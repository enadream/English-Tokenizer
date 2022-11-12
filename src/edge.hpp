#ifndef EDGE_HPP
#define EDGE_HPP

#include "node.hpp"
#include "datatypes.hpp"
#include "recorder.hpp"

class Node;

class Edge {
public:
	Node* pointedNode;
	Record* edgeInfo;

public:
	Edge();
	~Edge();
};

#endif
