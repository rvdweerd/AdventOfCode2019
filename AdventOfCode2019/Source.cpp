#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
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
void CreateGraphFromMaze(Maze_simple& maze, SimpGraph& graph, Pos startpos, std::string keys)
{
	std::vector<pathToKey> pathsToNewKeysFromCurrentPos;

	// get all available directions from startposition and initialize the queue for BFS
	std::queue<Pos> newPositionsQueue;
	std::string edgeStart = keys;
	std::vector<Pos> visited; visited.push_back(startpos);
	for (int dir : maze.GetAvailableDirectionsFromPos(startpos, keys))
	{
		Pos newpos = startpos.Add(dir);
		newpos.n++;
		newPositionsQueue.push(newpos);
	}
	maze.BFSforKeys(pathsToNewKeysFromCurrentPos, newPositionsQueue, visited, keys, 0);
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
			CreateGraphFromMaze(maze, graph, p.endpos, newNodeName);
		}
	}
	return;
}

void Day18()
{
	Maze_simple maze("Resources/day18input.txt");
	maze.PrintMaze();
	//NavigateMazeByHand(maze);
	std::string keys = "@";
	Pos startpos = maze.GetStartPos();
	maze.endkey = 'g';

	SimpGraph graph;
	graph.AddNode("@");
	CreateGraphFromMaze(maze, graph, startpos, keys);
	
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

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day18();
	
	while (!_kbhit());
	return 0;
}

