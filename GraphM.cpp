#include "GraphM.h"
#include <climits>

GraphM::GraphM()
{
	size = 0;
	isDirty = true;
	int temp = sizeof(data) / sizeof(*data);
	for (int i = 0; i < temp; i++)
	{
		for (int j = 0; j < temp; j++)
		{
			// set values to "infinity"
			T[i][j].dist = UINT_MAX;
			T[i][j].path = UINT_MAX;

			T[i][j].visited = false;

			C[i][j] = UINT_MAX;

		}
	}
}

//No 
GraphM::~GraphM()
{

}

//**************************************************************************
// SETUP FUNCTIONS

/*
	Builds a graph according to instructions found at the beginning of
	each Graph section (ended by 0 0 0 as a "null node" line) in a txt file.

	Begins by setting Graph size, then setting data array values to the # names
	found in the text file, then begins adding edges.
*/
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
	Inserts a directed edge that points _from_ to _to_ with a magnitude of _length_.
	If an existing edge already exists, the old edge is replaced with the new, and
	the function returns true. If there is no existing edge, returns false.

	Because this operation can be one in many, sets isDirty boolean to true when executed,
	and	findShortestPath is only performed at the end of any given sequence of remove edge.
*/
bool GraphM::insertEdge(const int& from, const int& to, const int& length)
{
	bool exists = edgeExists(from - 1, to - 1);
	C[from - 1][to - 1] = length;
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
	bool exists = edgeExists(from - 1, to - 1);
	C[from - 1][to - 1] = UINT_MAX;
	isDirty = true;
	return exists;
};


//**************************************************************************************
// CHECK FUNCTIONS

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





//*************************************************************************************
// DISPLAY AND DISPLAY HELPER FUNCTIONS


/*
	Displays the most length of the optimal path from _from_ to _to_,
	and also the names of the nodes that must be taken, if the path exists.
	If no path exists, returns a number of dashes.
*/
void GraphM::display(const int& from, const int& to)
{
	int f = from - 1;
	int t = to - 1;
	int index = 0;

	// Worst case scenario where best path requires all nodes, and sets to inf.
	unsigned int stack[MAX_NODE_SIZE] = { UINT_MAX };
	
	
	if (isDirty)
	{
		findShortestPath();
	}

	cout << "\t" << from << "\t" << to << ": ";

	if (T[f][t].dist != UINT_MAX)
	{
		cout << "\t" << T[f][t].dist << "\t\t" << f + 1;
		stack[index++] = from;

		//index value to traverse T forward and backward.
		int temp = t;

		// direct pointing is designated with UINT_MAX so only
		// source and destination are printed.
		while (T[f][temp].path != UINT_MAX)
		{
			cout << " " << T[f][temp].path + 1;
			stack[index++] = T[f][temp].path + 1;
			temp = T[f][temp].path;

		}

		stack[index] = to;
		cout << " " << to << endl;

		int end = index;
		index = 0;
		while (index <= end)
		{
			cout << data[stack[index]-1] << endl;
			index++;
		}
	}
	else
	{
		cout << "\t----" << endl;
	}
};

/*
	Prints node trajectories from _ to _, along with the shortest distance as calculated
	by Djikstra's algorithm, and the path the shortest distance represents.

	Though there may be paths with significantly fewer node traversals but equivalent
	distance, we will assume that differentiating between them is unnecessary
*/
void GraphM::displayAll()
{
	//check if the T table is up to date. If not, update.
	if (isDirty)
	{
		findShortestPath();
	}

	cout << endl;
	cout << "Description\t\t\tFrom Node \tToNode \t\tDijkstra's \t Path" << endl;
	
	// Display helper displays all optimal paths from one node to all other nodes.
	for (int i = 0; i < size; i++)
	{
		displayHelper(i);
	}
};

/*
	Performs grunt work for displayAll. Increments node values by 1 to conform to
	node naming standards in output.
*/
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

			//index value to traverse T forward and backward.
			int temp = i;

			// direct pointing is designated with UINT_MAX so only
			// source and destination are printed.
			while (T[nodeNumber][temp].path != UINT_MAX)
			{
				cout << " " << T[nodeNumber][temp].path + 1;
				temp = T[nodeNumber][temp].path;
			}

			cout << " " << i + 1 << endl;
		}
		else
		{
			cout << "---" << endl;
		}
	}
}


/*
void GraphM::pathPrint(const int& nodeNumber, const int& i)
{
	cout << T[nodeNumber][i].dist << "\t\t" << nodeNumber + 1;

	//index value to traverse T forward and backward.
	int temp = i;

	// direct pointing is designated with UINT_MAX so only
	// source and destination are printed.
	while (T[nodeNumber][temp].path != UINT_MAX)
	{
		cout << " " << T[nodeNumber][temp].path + 1;
		temp = T[nodeNumber][temp].path;
	}

	cout << " " << i + 1 << endl;
}
*/

/*
	Updates T multi-array with current values after all insert/remove edge operations
	have been performed. When completed, the T table is up to date and the isDirty
	boolean is switched to false to reflect its accuracy.

	Calls the recursive function graphTraversal to actually designate values.
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

/*
	Recursive function that treats the graph like a tree. Makes use of the T[][].visited
	boolean to implement a crude Stack structure.
	Calls updateTCell to check if the current distance is the most optimal pathway to
	"here" from "source"
*/
void GraphM::graphTraversal(const int& source, const int& here, const int& prev, const int& dist)
{
	updateTCell(source, here, prev, dist);
	for (int i = 0; i < size; i++)
	{
		if (C[here][i] != UINT_MAX && !T[source][here].visited)
		{
			int cumDist = dist + C[here][i];

			// organization treats graphTraversal as a stack that searches the graph.
			T[source][here].visited = true;
			graphTraversal(source, i, here, cumDist);
			T[source][here].visited = false;
		}
	}
}

/*
	Performs a check to see if an incoming pathway is the most optimal means of getting
	from "source" to "here".

	Updates a given cell with the smallest distance taken in from outside and the node that 
	immediately	preceded it

	If the there is no pathway (i.e. one degree of separation) UINT_MAX is set in T[][].path
	otherwise, points to the next cell in path sequence.
*/
void GraphM::updateTCell(const int& source, const int& here, const int& prev, const int& dist)
{
	if (T[source][here].dist >= dist && source != here)
	{
		T[source][here].dist = dist;
		if (source == prev)
		{
			T[source][here].path = UINT_MAX;
		}
		else
		{
			T[source][here].path = prev;
		}
	}
}