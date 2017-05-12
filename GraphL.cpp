
#include "GraphL.h"


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
	for (int i = 0; i < size; i++)
	{
		delAdjList(G[i]); // eventually sets edgeHead to NULL
		delete G[i].data;
		G[i].data = NULL;
	}
}

/*	buildGraph
Fills an empty instance of GraphL with usable data. Assumes that
the graph will not be reassigned, i.e. buildGraph will only be called
once during any GraphL's lifetime.

Given a file input, builds n number of GraphNodes and inserts them 
in sequence	to the GraphNode array G. After all GraphNodes have been
inserted, begins "pushing" directed EdgeNodes to related GraphNodes.


*/
void GraphL::buildGraph(ifstream& infile)
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
		getline(infile, temp);
		insertGNode(i, temp);
	}

	//assumes that the line begins with numbers and the line only ever contains
	//numeric characters or a space
	while (getline(infile, temp))
	{
		// if terminating case, stop buildGraph
		if (temp == "0 0")
		{
			return;
		}

		int spacePos = temp.find(" ");
		int fromNode = stoi(temp.substr(0, spacePos));
		int toNode = stoi(temp.substr(spacePos));

		insertENode(G[fromNode-1], toNode-1);
	}

	return;
}

/* insertGNode
Receives raw data to construct a new piece of NodeData, and changes the values
of G[index] to reflect the new data. If there exists a GraphNode at the current
location, overwrites. (This should never happen as insertGNode should only be
called once per array cell during the buildGraph function.)

Preconditions:

Postconditions:

*/
bool GraphL::insertGNode(const int &index, const string &data)
{
	delete G[index].data;
	G[index].data = NULL;
	NodeData* temp = new NodeData(data);
	G[index].data = temp;
	temp = NULL;
	return true;
}

/* insertENode
Is given a graphNode to insert into, and a value representing the node
to which the edgeNode is pointing. The new EdgeNode is always inserted at
the beginning of the list, and the code assumes that there are no redundant
edges.

Preconditions:

Postconditions:

*/
void GraphL::insertENode(GraphNode& recipient, const int &toNode)
{
	EdgeNode* temp = new EdgeNode;
	temp->adjGraphNode = toNode;
	temp->nextEdge = NULL;

	if (recipient.edgeHead == NULL)
	{
		recipient.edgeHead = temp;
		temp = NULL;
	}
	else
	{
		temp->nextEdge = recipient.edgeHead;
		recipient.edgeHead = temp;
	}
}

/* unvisit
Helper function that is called at the end of every depthFirstSearch to
reset all graphNode visited booleans for future DFSes.

Preconditions:

Postconditions:

*/
void GraphL::unvisit()
{
	for (int i = 0; i < MAXNODES; i++)
	{
		G[i].visited = false;
	}
}

/* delAdjList
Deletes a given graphNode's adjacency list. Functions similarly to the
insertion, deletes the head and "pulls" the subsequent nodes in to replace
it.
	
*/
void GraphL::delAdjList(GraphNode toDelete)
{
	EdgeNode* temp = toDelete.edgeHead;
	while (toDelete.edgeHead != NULL)
	{
		toDelete.edgeHead = toDelete.edgeHead->nextEdge;
		delete temp;
		temp = toDelete.edgeHead;
	}
}

/* displayGraph
Prints out all available graphNodes contained in the range 0-size. Node counting
begins at 1, and each list of edges is prefaced with the graphNode data.

*/
void GraphL::displayGraph()
{
	cout << "--Graph:" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << "Node " << i + 1 << "\t\t" << *G[i].data << endl;
		printEdges(i);
	}
	cout << endl << endl;
}

/* printEdge
Helper function that prints all edges of a given GraphNode. Iterates through
the linked list of edges and prints each edge on seperate lines.

Preconditions:

Postconditions:

*/
void GraphL::printEdges(const int& Gindex) const
{
	EdgeNode* index = G[Gindex].edgeHead;
	if (index == NULL)
	{
		cout << "\tedge -- --" << endl;
	}
	while (index != NULL)
	{
		cout << " \tedge " << Gindex+1 << " " << index->adjGraphNode+1 << endl;
		index = index->nextEdge;
	}
}

/* depthFirstSearch
Public access function to print out the Depth First Search output. Calls helper
function to perform search recursively, then calls unvisit() to reset visited
booleans of graphNodes.

Does not assume all nodes are connected, and will thus iterate through the
array until "size" cells in.
*/
void GraphL::depthFirstSearch()
{
	cout << "Depth-first ordering: ";
	for (int i = 0; i < size; i++)
	{
		dfsHelper(i);
	}
	cout << endl;
	unvisit();
}

/* dfsHelper
Recursive function that traverses the graph as if it were a tree and prints
node numbers in preorder. If the node is already visited, returns out of the
function and prints nothing/does not traverse that nodes subtree.
*/
void GraphL::dfsHelper(const int& gIndex)
{
	if (G[gIndex].visited == true)
	{
		return;
	}

	cout << gIndex + 1 << " ";
	G[gIndex].visited = true;

	EdgeNode* temp = G[gIndex].edgeHead;
	while (temp != NULL)
	{
		dfsHelper(temp->adjGraphNode);
		temp = temp->nextEdge;
	}

}