#include "GraphM.h"

GraphM::GraphM(void)
{
	size = 0;
	isDirty = true;
	int temp = sizeof(data) / sizeof(*data);
	for (int i = 0; i < temp; i++)
	{
		for (int j = 0; j < temp; j++)
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
	string temp = "";
	getline(fromFile, temp);

	if (temp == "")
	{
		return;
	}

	size = stoi(temp);

	//assumes there are exactly _size_ nodes for name data.
	for (int i = 0; i < size; i++)
	{
		data[i].setData(fromFile);
	}

	//assumes that the line begins with numbers and the line only ever contains
	//numeric characters or a space
	while (getline(fromFile, temp))
	{
		if (isNullNode(temp))
		{
			return;
		}
		strToNodeHelper(temp);
	}
	
	return;
};

/*
	Assumes every line passed in is only composed of 3 different "words" made 
	of numbers, each separated by at least 1 space.
*/
void GraphM::strToNodeHelper(const string& toConvert)
{
	int from, to, length, index, index2 = 0;

	// finds position of first space and second space.
	index = toConvert.find(' ');
	index2 = toConvert.find_last_of(' ');

	// cast substrings as integers
	from = stoi(toConvert.substr(0, index));
	to = stoi(toConvert.substr(index + 1, index2 - index - 1));
	length = stoi(toConvert.substr(index2 + 1));


	// inserts edge. If edge exists, overwrite old and cout error message.
	if (insertEdge(from, to, length)) 
	{ 
		cout << "Overwriting edge " << from << "-" << to << endl; 
	}
}

/*
	If the line contains a "null terminating node" or "0 0 0" value, return true to
	break to next step. Assuming that input is formatted in such a way that 0 is
	only present at position '0' as a terminating "node"
*/
bool GraphM::isNullNode(const string& toCheck) const
{
	if (toCheck.at(0) == '0')
	{
		return true;
	}
	return false;
}

	//Helper function to determine if the edge exists.
bool GraphM::edgeExists(const int& from, const int& to) const
{
	return (C[from-1][to-1] != UINT_MAX);
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
	bool exists = edgeExists(from-1, to-1);
	C[from-1][to-1] = length;
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
	bool exists = edgeExists(from-1, to-1);
	C[from-1][to-1] = UINT_MAX;
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