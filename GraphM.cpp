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
			T[i][j].path = UINT_MAX;
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

void GraphM::display(const int& from, const int& to)
{
	if (isDirty)
	{
		findShortestPath();
	}

	cout << from << " " << to << endl;
};

/*
	Prints node information (i.e. Name), then --	
	Prints node trajectories from _ to _, along with the shortest distance as calculated
	by Djikstra's algorithm, and the path the shortest distance represents.
*/
void GraphM::displayAll()
{
	if (isDirty)
	{
		findShortestPath();
	}

	cout << "Description\t\t\tFrom Node \tToNode \t\tDijkstra's \t Path" << endl;
	
	for (int i = 0; i < size; i++)
	{
		displayHelper(i);
	}
};

void GraphM::displayHelper(const int& nodeNumber) const
{
	// print name of node
	cout << data[nodeNumber] << endl;

	// print all outgoing node connections
	for (int i = 0; i < size; i++)
	{
		if (i == nodeNumber) { continue; };
		cout << "\t\t\t\t" << nodeNumber + 1 << "\t\t" << i + 1 << "\t\t";
		if (T[nodeNumber][i].dist != UINT_MAX)
		{
			cout << T[nodeNumber][i].dist << "\t\t" << nodeNumber + 1;

			cout << endl;

		}
		else
		{
			cout << "---" << endl;
		}
	}
}

/*
	Updates T multi-array with current values after all insert/remove edge operations
	have been performed. When completed, the T table is up to date and the isDirty
	boolean is switched to false to reflect its accuracy.
*/













void GraphM::findShortestPath()
{
	// iterate through every node and perform Dijkstra's algorithm to find shortest
	// paths to each.
	for (int i = 0; i < size; i++)
	{
		graphTraversal(i, i, INT_MAX, 0);
	}

	isDirty = false;
};

void GraphM::graphTraversal(const int& source, const int& here, const int& prev, const int& dist)
{
	updateTCell(source, here, prev, dist);
	for (int i = 0; i < size; i++)
	{
		if (C[here][i] != UINT_MAX && !T[source][here].visited)
		{
			int cumDist = dist + C[here][i];
			T[source][here].visited = true;
			graphTraversal(source, i, here, cumDist);
			T[source][here].visited = false;
		}
	}
}

/*
	updates a given cell with the smallest distance taken in from outside and the node that 
	immediately	preceded it
*/
void GraphM::updateTCell(const int& source, const int& here, const int& prev, const int& dist)
{
	if (T[source][here].dist >= dist && source != here)
	{
		T[source][here].dist = dist;
		T[source][here].path = prev;
	}
}

/* 
	Returns shortest path to nonvisited node and records minimum distances in table
	T. Also records visited nodes.
*/
int GraphM::shortestCurrentPath(int fromNode)
{
	int min = UINT_MAX;
	int toGo = UINT_MAX;

	for (int v = 0; v < size; v++)
	{
		if (C[fromNode][v] <= min && !T[fromNode][v].visited)
		{
			min = C[fromNode][v];
			toGo = v;
		}
	}
	return toGo;
}