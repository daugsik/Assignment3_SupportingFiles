#include "GraphM.h"

GraphM::GraphM(void)
{
	size = 0;
	for (int i = 1; i < sizeof(data); i++)
	{
		for (int j = 1; j < sizeof(data); j++)
		{
			T[i][j].dist = UINT_MAX;
			T[i][j].path = 0;
			T[i][j].visited = false;

			C[i][j] = UINT_MAX;
		}
	}
}

GraphM::~GraphM(void)
{

}

void GraphM::buildGraph(ifstream& fromFile)
{
	size = fromFile.get();

	for (int i = 1; i <= size; i++)
	{
		data[i].setData(fromFile);
	}

	string holder = "";

	while (holder = fromFile.getLine())
	{

	}

};

/*
	If the line contains a "null terminating node" or "0 0 0" value, return true to
	break to next step.
*/
bool GraphM::isNullNode(const string& toCheck) const
{
	return true;
}

bool GraphM::edgeExists(const int& from, const int& to) const
{
	return (C[from][to] != UINT_MAX);
}

/*
	Inserts a directed edge that points _from_ to _to_ with a magnitude of _length_.
	If an existing edge already exists, the old edge is replaced with the new, and
	the function returns true. If there is no existing edge, returns false.

	Because this operation can be one in many, sets isDirty boolean to true when executed, 
	and findShortestPath is only performed at the end of any given sequence of remove edge.
*/
bool GraphM::insertEdge(const int& from, const int& to, const int& length)
{
	bool exists = edgeExists(from, to);
	C[from][to] = length;
	isDirty = true;
	return exists;
};

/*
	Removes a preexisting edge that points from node _from_ to node _to_ by setting
	the distance variable to 'infinity' (i.e. max uint). Because this operation can
	be one in many, sets isDirty boolean to true when executed, and findShortestPath
	is only performed at the end of any given sequence of remove edge.
	Returns true if an edge is found and is successfully removed.
	Returns false if no edge exists to be removed.
*/
bool GraphM::removeEdge(const int& from, const int& to)
{
	bool exists = edgeExists(from, to);
	C[from][to] = UINT_MAX;
	isDirty = true;
	return exists;
};

void GraphM::display(const int& from, const int& to) const
{

};

/*
	Prints node information (i.e. Name), then --	
	Prints node trajectories from _ to _, along with the shortest distance as calculated
	by Djikstra's algorithm, and the path the shortest distance represents.
*/
void GraphM::displayAll() const
{

};

/*
	Updates T multi-array with current values after all insert/remove edge operations
	have been performed. When completed, the T table is up to date and the isDirty
	boolean is switched to false to reflect its accuracy.
*/
void GraphM::findShortestPath()
{

	isDirty = false;
};