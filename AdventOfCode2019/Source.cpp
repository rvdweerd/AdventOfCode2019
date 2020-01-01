#include "IncludesUsed.h"
#include "PastDaysSolutions.h"

// THIS CODE WAS MY FIRST TRY AND (ALTHOUGH IT CAN BE MADE TO WORK-NEED TO FIX THE MEMOIZING/CACHING) I ABANDONED THE APPROACH: TOO COMPLEX AND SLOW
/*
#include "SimpGraph.h"

void NavigateMazeByHand(Maze_simple maze_tmp)
{
	char ch=0;
	std::vector<char> keys;
	for (; ch != 'q'; )
	{
		std::cout << "Move (1) North, (2) South, (3) West, (4) East, (q)uit >";
		std::cin >> ch;
		int dir = int(ch - '0');
		if (maze_tmp.Available(dir, keys))
			maze_tmp.MoveTo(dir, keys);
		for (char c : keys)
		{
			std::cout << c;
		}
		std::cout << std::endl;
		maze_tmp.PrintMaze();
	}
}
void DijkstraTest()
{
	SimpGraph graph;
	graph.AddNode("start");
	graph.AddNode("a");
	graph.AddNode("b");
	graph.AddNode("c");
	graph.AddNode("d");
	graph.AddNode("e");
	graph.AddNode("f");
	graph.AddTwoWayConnection("start", "a", 2);
	graph.AddTwoWayConnection("a", "b", 6);
	graph.AddTwoWayConnection("b", "c", 10);
	graph.AddTwoWayConnection("c", "e", 14);
	graph.AddTwoWayConnection("c", "d", 24);
	graph.AddTwoWayConnection("e", "d", 38);
	graph.AddTwoWayConnection("e", "f", 6);
	auto vec = graph.findShortestPath("start", "f");
	for (auto a : vec)
	{
		std::cout << a->finish->name << "->";
	}
}
void CreateGraphFromMaze(Maze_simple& maze, SimpGraph& graph, Pos startpos, std::string keys, std::map<std::string, std::vector<pathToKey>>& cache, int& cacheused)
{
	std::vector<pathToKey> pathsToNewKeysFromCurrentPos;
	
	std::string orderedkeys = keys;
	std::sort(orderedkeys.begin(), orderedkeys.end());
	orderedkeys += std::to_string(startpos.x);
	orderedkeys += "_";
	orderedkeys += std::to_string(startpos.y);
	
	if (cache.find(orderedkeys) == cache.end())
	{
		maze.BFSforKeys(pathsToNewKeysFromCurrentPos, startpos, keys, 0, cache);
	}
	else
	{
		pathsToNewKeysFromCurrentPos = cache[orderedkeys];
		cacheused++;
	}
	   	
	for (pathToKey p : pathsToNewKeysFromCurrentPos)
	{
		//if (p.endKeyChar == maze.endkey)
		if (keys.size() == maze.nKeys)
		{
			std::string endNodeName = "end"; //endNodeName += p.endKeyChar;
			graph.AddNode(endNodeName);
			graph.AddOneWayConnection(keys, endNodeName, p.distance);
		}
		else
		{
			std::string newNodeName = keys; newNodeName.append(1, p.endKeyChar);
			graph.AddNode(newNodeName);
			graph.AddOneWayConnection(keys, newNodeName, p.distance);
			p.endpos.n = 0;
			CreateGraphFromMaze(maze, graph, p.endpos, newNodeName, cache, cacheused);
		}
	}
	return;
}
void Day18abandoned()
{
	// Although this is a working implementation of a solution using Dijkstra's algo, it is overly complicated and way too slow.
	// I chose to abandon this approach entirely and simplfy the solution to a basic BFS. Code kept in for own learning documentation
	Maze_simple maze("Resources/day18input.txt");
	maze.PrintMaze();
	//NavigateMazeByHand(maze);
	std::string keys = "@";
	Pos startpos = maze.GetStartPos();
	maze.endkey = 'g';

	SimpGraph graph;
	graph.AddNode("@");
	std::map<std::string , std::vector<pathToKey>> cache;
	int cacheused = 0;
	CreateGraphFromMaze(maze, graph, startpos, keys, cache,cacheused);
	
	//Find shortest path (vector of arcs)
	//std::string endpoint = "@"; endpoint += maze.endkey;
	std::vector<SimpGraph::Arc*> answer = graph.findShortestPath("@", "end");
	int pathlen = SimpGraph::getPathCost(answer);
	std::cout << "Shortes path takes " << pathlen << " steps.\n";
	for (auto a : answer)
	{
		std::cout << a->finish->name << ", ";
	}
}
*/
// END OF ABANDONED CODE

std::string SerializeIntoString(Pos pos) // used to enable a hash lookup of visited nodes in the BFS
{
	std::sort(pos.keys.begin(), pos.keys.end());
	pos.keys.append(std::to_string(pos.x));
	pos.keys.append("_");
	pos.keys.append(std::to_string(pos.y));
	return pos.keys;
}
std::vector<Pos> FindNeighbors(const std::vector<char>& field, const int& fieldWidth, const Pos& pos, const std::set<std::string>& visited)
{
	std::vector<Pos> availablePositions;
	int newX, newY;
	for (int dir = 1; dir < 5; dir++)
	{
		if (dir == 1) // NORTH
		{
			newX = pos.x;
			newY = pos.y - 1;
		}
		else if (dir == 2) // SOUTH
		{
			newX = pos.x;
			newY = pos.y + 1;
		}
		else if (dir == 3) // WEST
		{
			newX = pos.x -1;
			newY = pos.y;
		}
		else if (dir == 4) // EAST
		{
			newX = pos.x + 1;
			newY = pos.y;
		}
		
		char ch = field[newY * fieldWidth + newX];
		if ((ch == '#') || // If neighbor is [wall] or [door without key] don't include
			(ch > 64 && ch <= 90 && (pos.keys.find(std::tolower(ch)) == std::string::npos)) )
		{
		}
		else // Else, include neighbor, if not already visited, add key if found
		{
			std::string newkeys = pos.keys;
			if (ch > 96 && ch <= 122 && (newkeys.find(ch) == std::string::npos)) // If new key is found, add it to newpos.keys
			{
				newkeys += ch; // This will retain the order in which the keys are found. We sort when serializing
			}
			Pos newpos = { newX,newY,pos.n+1, newkeys };
			if (visited.find(SerializeIntoString(newpos)) == visited.end())
			{
				availablePositions.push_back(newpos);
			}
		}
	}
	return availablePositions;
}
void LoadField(std::string filename, std::vector<char>& field, int& fieldWidth, Pos& startpos, int& nKeys)
{
	fieldWidth = 1; // we don't know this yet, this is the starting assumption
	bool fieldWidthSet = false;
	std::ifstream in(filename);
	while (!in.eof())
	{
		char ch;
		int i = 0;
		std::string str;
		for (ch = in.get(); !in.eof(); ch = in.get())
		{
			if (ch == '\n')
			{
				if (!fieldWidthSet)
				{
					fieldWidth = i;
					fieldWidthSet = true;
				}
			}
			else
			{
				if (ch == '@')
				{
					startpos.y = i / fieldWidth;
					startpos.x = i - startpos.y * fieldWidth;
				}
				field.push_back(ch);
				i++;
			}
		}
	}

	// Scan for number of keys in the maze
	nKeys = 0; // in case not initialized by caller
	for (char c : field)
	{
		if (c >= 97 && c < 123)
		{
			nKeys++;
		}
	}

}
void Day18a()
{
	// Initialize
	std::vector<char> field;
	int fieldWidth;
	Pos startpos;
	int nKeys;
	LoadField("Resources/day18input.txt", field, fieldWidth, startpos, nKeys);

	// BFS to find all keys
	std::queue<Pos> queue;
	std::set<std::string> visited;
	queue.push(startpos);
	visited.insert(SerializeIntoString(startpos));
	

	while (!queue.empty())
	{
		Pos currentpos = queue.front(); queue.pop();
		std::vector<Pos> neighbors = FindNeighbors(field, fieldWidth, currentpos, visited);
		for (Pos p : neighbors)
		{
			if (p.keys.size() == nKeys)
			{
				std::cout << "Last step to: (" << p.x << "," << p.y << "), keys = " << p.keys << std::endl;
				std::cout << "All keys obtained, " << p.n << "steps taken.";
				while (!queue.empty()) queue.pop();
				break;
			}
			visited.insert(SerializeIntoString(p));
			queue.push(p);
		}
	}
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day18a();
	
	while (!_kbhit());
	return 0;
}

