#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
#include "SimpGraph.h"

void InitializeDay18(SimpGraph& graph)
{
	graph.AddNode("Atlanta");
	graph.AddNode("Boston");
	graph.AddNode("Chicago");
	graph.AddNode("Dallas");
	graph.AddNode("Denver");
	graph.AddNode("Los Angeles");
	graph.AddNode("New York");
	graph.AddNode("Portland");
	graph.AddNode("San Francisco");
	graph.AddNode("Seattle");
	graph.AddTwoWayConnection("Atlanta", "Chicago", 599);
	graph.AddTwoWayConnection("Atlanta", "Dallas", 725);
	graph.AddTwoWayConnection("Atlanta", "New York", 756);
	graph.AddTwoWayConnection("Boston", "New York", 191);
	graph.AddTwoWayConnection("Boston", "Seattle", 2489);
	graph.AddTwoWayConnection("Chicago", "Denver", 907);
	graph.AddTwoWayConnection("Dallas", "Denver", 650);
	graph.AddTwoWayConnection("Dallas", "Los Angeles", 1240);
	graph.AddTwoWayConnection("Dallas", "San Francisco", 1468);
	graph.AddTwoWayConnection("Denver", "San Francisco", 954);
	graph.AddTwoWayConnection("Portland", "San Francisco", 550);
	graph.AddTwoWayConnection("Portland", "Seattle", 130);
	graph.PrintAdjacencyList();
}

void Day18()
{
	SimpGraph graph;
	InitializeDay18(graph);
	graph.BFS("Boston");
	
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

