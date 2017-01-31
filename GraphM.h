/*
	Header for Part 1 of Homework 2.
	Provides prototypes for Djikstra's Algorithm graph manipulation
*/
#pragma once
#include "NodeData.h"
#define MAX_NODE_SIZE 100
#include <fstream>

class GraphM
{
public:
	GraphM();
	~GraphM();
	void buildGraph(ifstream& fromFile);
	bool insertEdge(const int& from, const int& to, const int& length);
	bool removeEdge(const int& from, const int& to);

	void display(const int& from, const int& to);
	void displayAll();
	void findShortestPath();

private:
	int size;
	bool isDirty;		// checks to see if structure was changed before
						// performing a findShortestPath operation.
	NodeData data[MAX_NODE_SIZE];
	unsigned int C[MAX_NODE_SIZE][MAX_NODE_SIZE];

	struct TableType
	{
		bool visited;
		unsigned int dist;
		int path;
	};

	TableType T[MAX_NODE_SIZE][MAX_NODE_SIZE];

	bool edgeExists(const int& from, const int& to) const;
	bool isNullNode(const string& toCheck) const;
	void strToNodeHelper(const string& toConvert);
	void displayHelper(const int& nodeNumber) const;
	void updateTCell(const int& source, const int& destination, const int& prev, const int& dist);
	void graphTraversal(const int& source, const int& here, const int& prev, const int& dist);
	void pathPrint(const int& nodeNumber, const int& i) const;
};