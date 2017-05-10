
#include "GraphL.h"

/*
GraphL::GraphL()
{
	size = 0;
	for (int i = 0; i < MAXNODES; i++)
	{
		G[i].edgeHead = NULL;
		G[i].data = NULL;
		G[i].visited = NULL;
	}
}

GraphL::~GraphL()
{

}


void GraphL::buildGraph(std::ifstream& infile)
{
	string temp = "";
	getline(infile, temp);

	if (temp == "")
	{
		return;
	}
	
	//reads in first line which should be graph size
	size = stoi(temp);

	//assumes there are exactly _size_ number of names.
	for (int i = 0; i < size; i++)
	{
		data[i].setData(infile);
	}

	//assumes that the line begins with numbers and the line only ever contains
	//numeric characters or a space
	while (getline(infile, temp))
	{
		
	}

	return;
}


void GraphL::displayGraph()
{

}


void GraphL::depthFirstSearch()
{

}

*/