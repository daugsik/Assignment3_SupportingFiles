#pragma once
/*
	Header for Part 2 of Homework 2.
	Provides prototypes for a depth-first search of any given graph.
*/

#include "NodeData.h"
#include <fstream>
#define MAXNODES 100

class GraphL
{
public:
	GraphL();
	~GraphL();

	void buildGraph(std::ifstream& infile);
	void displayGraph();
	void depthFirstSearch();

private:
	int size;
	struct EdgeNode;

	struct GraphNode 
	{
		EdgeNode* edgeHead;
		NodeData* data;
		bool visited;
	};

	GraphNode G[MAXNODES];

	struct EdgeNode
	{
		int adjGraphNode;
		EdgeNode* nextEdge;
	};
};
