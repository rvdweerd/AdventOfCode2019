#include "SimpGraph.h"
#include <iostream>

void SimpGraph::AddNode(std::string name)
{
	Node* newnode = new Node(name);
	nodeMap[name] = newnode;
	nodes.insert(newnode);
}

void SimpGraph::AddArc(Node* start, Node* finish, int cost)
{
	Arc* newarc = new Arc(start, finish, cost);
	start->arcs.insert(newarc);
	arcs.insert(newarc);
}

void SimpGraph::AddOneWayConnection(std::string n1, std::string n2, int c)
{
	if (nodeMap.find(n1) == nodeMap.end())
	{
		AddNode(n1);
	}
	if (nodeMap.find(n2) == nodeMap.end())
	{
		AddNode(n2);
	}
	AddArc(nodeMap[n1], nodeMap[n2], c);
}
void SimpGraph::AddTwoWayConnection(std::string n1, std::string n2, int c)
{
	if (nodeMap.find(n1) == nodeMap.end())
	{
		AddNode(n1);
	}
	if (nodeMap.find(n2) == nodeMap.end())
	{
		AddNode(n2);
	}
	AddArc(nodeMap[n1], nodeMap[n2], c);
	AddArc(nodeMap[n2], nodeMap[n1], c);
}

void SimpGraph::PrintAdjacencyList()
{
	for (Node* n : nodes)
	{
		std::cout << n->name << " (" << n->arcs.size() << ") -> ";
		for (Arc* a : n->arcs)
		{
			std::cout << a->finish->name << "(" << a->cost << "),";
		}
		std::cout << std::endl;
	}
}

void SimpGraph::DFS(std::string startname)
{
	if (nodeMap.find(startname) != nodeMap.end())
	{
		DFS(nodeMap[startname]);
	}
}

void SimpGraph::DFS(Node* startnode)
{
	visited.clear();
	visitUsingDFS(startnode);
}

void SimpGraph::BFS(std::string startname)
{
	if (nodeMap.find(startname) != nodeMap.end())
	{
		BFS(nodeMap[startname]);
	}
}

void SimpGraph::BFS(Node* startnode)
{
	visited.clear();
	while (!tovisit.empty()) tovisit.pop();
	tovisit.push(startnode);
	visited.insert(startnode);
	visitUsingBFS();
}

void SimpGraph::VisitFunction1(Node* node)
{
	std::cout << node->name<< " visited.\n";
}

void SimpGraph::visitUsingDFS(Node* node)
{
	if (visited.find(node) == visited.end())
	{
		VisitFunction1(node);
		visited.insert(node);
		for (Arc* a : node->arcs)
		{
			visitUsingDFS(a->finish);
		}
	}
	else
	{
		return;
	}
}

void SimpGraph::visitUsingBFS()
{
	while (!tovisit.empty())
	{
		Node* nextnode = tovisit.front();
		tovisit.pop();
		VisitFunction1(nextnode);
		
		for (Arc* a : nextnode->arcs)
		{
			if (visited.find(a->finish) == visited.end())
			tovisit.push(a->finish);
			visited.insert(a->finish);
		}
	}
	return;
}
