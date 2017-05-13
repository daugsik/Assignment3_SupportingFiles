#pragma once
/*
	Header for Part 2 of Homework 2.
	Provides prototypes for a depth-first search of any given graph.
*/

#include "NodeData.h"
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
	struct EdgeNode;
	struct GraphNode 
	{
		EdgeNode* edgeHead;
		NodeData* data;
		bool visited;
	};

	struct EdgeNode
	{
		int adjGraphNode;
		EdgeNode* nextEdge;
	};

	int size;
	GraphNode G[MAXNODES];

	bool insertGNode(const int&, const string&);
	void insertENode(GraphNode&, const int &);
	void unvisit();
	void printEdges(const int&) const;
	void delAdjList(GraphNode);
	void dfsHelper(const int&);
};
