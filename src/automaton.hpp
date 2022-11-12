#ifndef AUTOMATON_HPP
#define AUTOMATON_HPP

#include "node.hpp"
#include "recorder.hpp"
#include <vector>

class Automaton {
private:
	// Q : Finite set of states.
	// S : Finite set of alphabet.
	// T : Transition Functions.
	// q : Initial state.
	// F : Set of Final States.

	Node* m_startNode; // q
	std::vector<Node*> m_nodes; // Q
	std::vector<Node*> m_edges; // T

	RecordList alphabetRecords; // S

	// std::vector<Node*> m_finalNodes; // F

public:
	Automaton();
	~Automaton();

	void DefineStartNode(Node* node);
	void AddNode(Node* node);
	void RemoveNode(Node* node);
	void StartAutomaton(const char& input_string);
};


#endif

