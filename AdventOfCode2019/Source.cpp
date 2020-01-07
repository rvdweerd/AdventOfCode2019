#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
#include "Day18HelperFunctions.h"
#include "Day18_Old Working.h"

void Day18a()
{
	// Init global data structs===========
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::map<char, int> keyIndices;
	std::map<char, Coi2> keyFieldCoordinates;
	// Load the field & ------------------
	Coi2 pos0 = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys, keyIndices, keyFieldCoordinates);
	PrintField(field, fieldWidth);
	// Initialize for BFS ----------------
	KeyPosition P0 = { {'@',keyFieldCoordinates['@']},"@",0,"@" };
	std::set<std::string> visitedKeys;
	std::queue<KeyPosition> keyQueue;
	keyQueue.push(P0);
	std::map<std::string,CacheInfo> cache;
	DiagnosticData diagData;
	// End Init ==========================

	// Main BFS loop
	std::vector<std::pair<std::string, int>> result;
	while (!keyQueue.empty())
	{
		if (keyQueue.size() > diagData.MaxQueueSize_OuterBFS) diagData.MaxQueueSize_OuterBFS = keyQueue.size();
		diagData.loopCount_OuterBFS++;
		KeyPosition curKeyPos = keyQueue.front(); keyQueue.pop();
		std::vector<std::pair<Key, int>> newKeys = GetAvailableKeyPositions(curKeyPos, field, fieldWidth, visitedKeys, cache, diagData);
		
		std::vector<KeyPosition> newPositions;
		for (std::pair<Key, int> p : newKeys)
		{
			std::string newkeys = curKeyPos.keys + p.first.keychar;
			std::sort(newkeys.begin(), newkeys.end());
			KeyPosition newKeyPosition = { p.first , newkeys, curKeyPos.steps + p.second, curKeyPos.path + p.first.keychar };
			newPositions.push_back(newKeyPosition);
		}

		for (KeyPosition p : newPositions)
		{
			if (p.keys.size() == nKeys) // end condition
			{
				//std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
				result.push_back({ p.path,p.steps });
			}
			else
			{
				visitedKeys.insert(Hash(p));
				keyQueue.push(p);
			}
		}
	}

	// Display Results:
	std::cout << "\n\nResults found.\n";
	std::cout << "==============\n";
	std::cout << "Number of keys found: " << nKeys << '\n';
	std::cout << "Number of unique routes: " << result.size() << '\n';
	
	std::vector<std::pair<std::string, int>>::iterator max_it = std::max_element(result.begin(), result.end(), route_compare);
	std::vector<std::pair<std::string, int>>::iterator min_it = std::min_element(result.begin(), result.end(), route_compare);
	std::cout << "Range of route lengths found: [" << min_it->second << "..." << max_it->second<<"]\n";
	std::cout << "Example of shortest path: " << min_it->first << '\n';
	std::cout << "Example of longest path : " << max_it->first << '\n';

	std::cout << "\nAlgorithm diagnostics: \n";
	std::cout << "====================== \n";
	std::cout << "Outer BFS loop (key-to-key): \n";
	std::cout << "> Maximum queue size: "<< diagData.MaxQueueSize_OuterBFS<< '\n';
	std::cout << "> Loop count: " << diagData.loopCount_OuterBFS<<'\n';
	std::cout << "Inner BFS loop (cell-to-cell): \n";
	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_InnerBFS<< '\n';
	std::cout << "> Loop count: " << diagData.loopCount_InnerBFS<<'\n';
	std::cout << "\nSize of key-to-key cache (#nodes in keyGraph): " << cache.size();
	
	//std::cout << "\n(press Enter to show keyGraph adjacency list)\n\n";
	//std::cin.get();
	std::cout << "\n\nAdjacency list (Node name is keylocation + keys_owned)\n";
	std::cout << "NODE         ARCS\n";
	std::cout << "======================================================\n";
	for (auto e : cache)
	{
		std::cout << std::left<< std::setw(nKeys+2) << e.first;
		std::cout << "->  ";
		for (auto p : e.second.cachedAvailablePositions)
		{
			std::cout << p.first.keychar << "[" << p.second << "], ";
		}
		std::cout << '\n';
	}
	return;
}
void Day18b()
{
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

