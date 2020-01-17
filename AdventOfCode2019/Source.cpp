#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
//#include "Day20HelperFunctions.h"
#include "Day18HelperFunctions.h"

int PathCost(const std::vector<KeyPosition4D>& path)
{
	if (path.size() > 0)
	{
		/*int count = 0;
		for (auto pos : path)
		{
			count += pos.steps;
		}*/
		return (path.end()-1)->steps - path[0].steps;
	}
	return 0;

}

struct GreaterPathCost
{
	// Dit is een compare function die gebruikt wordt voor het sorteren (prioriteren) in de priority_map class voor de toepassing 
	// in het Dijkstra algorithme
	bool operator()(const std::vector<KeyPosition4D>& lhs, const std::vector<KeyPosition4D>& rhs) const
	{
		return PathCost(lhs) > PathCost(rhs);
	}
};

std::vector<KeyPosition4D> GetAllAvailableKeyPositions(KeyPosition4D start, 
	const std::vector<char>& field,
	const int& fieldWidth,
	std::set<std::string>& visitedKeys,
	std::map<std::string, CacheInfo4D>& cache,
	DiagnosticData& diagData)
{
	std::vector<std::vector<std::pair<Key, int>>> newKeys = GetAvailableKeyPositionsForSubFields(start, field, fieldWidth, visitedKeys, cache, diagData);
	std::vector<KeyPosition4D> newPositions;
	if (!newKeys.size() == 0)
	{
		for (int i = 0; i < 4; i++) // loop the 4 subFields // for (std::vector<std::pair<Key, int>> pVec : newKeys)
		{
			std::vector<std::pair<Key, int>> pVec = newKeys[i];
			for (std::pair<Key, int> p : pVec)
			{
				KeyPosition4D newKeyPosition = start;
				newKeyPosition.key[i] = p.first;
				newKeyPosition.steps += p.second;
				std::string newkeys = start.keys + p.first.keychar;
				std::sort(newkeys.begin(), newkeys.end());
				newKeyPosition.keys = newkeys;
				newKeyPosition.path += p.first.keychar;
				newPositions.push_back(newKeyPosition);
			}
		}
	}
	return newPositions;
}

std::vector<KeyPosition4D> FindShortestPath(KeyPosition4D start, KeyPosition4D finish, 
	const std::vector<char>& field,
	const int& fieldWidth,
	std::set<std::string>& visitedKeys,
	std::map<std::string, CacheInfo4D>& cache,
	DiagnosticData& diagData)
{
	std::vector<KeyPosition4D> path;
	std::priority_queue< std::vector<KeyPosition4D>, std::vector<std::vector<KeyPosition4D>>, GreaterPathCost> queue;
	std::map<std::string, int> fixed;

	while (start.keys != finish.keys)
	{
		//if (fixed.find(Hash(start)) == fixed.end())
		//if (fixed.find(start.keys) == fixed.end())
		if (fixed.find(start.keys) == fixed.end())
		{
			//fixed.insert({ Hash(start), PathCost(path) });
			//fixed.insert({ start.keys, PathCost(path) });
			fixed.insert({ start.keys, PathCost(path) });
			std::vector<KeyPosition4D> newKeyPositions = GetAllAvailableKeyPositions(start, field, fieldWidth, visitedKeys, cache, diagData);
			for (KeyPosition4D p : newKeyPositions)
			{
				path.push_back(p);
				queue.push(path);
				path.erase(path.end() - 1);
			}
		}
		else
		{
			if (start.steps < fixed.find(start.keys)->second)
			{
				int k = 0;
			}
		}
		if (queue.empty())
		{
			path.clear();
			return path;
		}
		path = queue.top(); queue.pop();
		start = *(path.end()-1);
	}
	//std::cin.get();
	return path;
}


void Day18c()
{
	// Init global data structs===========
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::map<char, int> keyIndices;
	std::map<char, Coi2> keyFieldCoordinates;
	std::vector<std::vector<char>> subFields; // 0 (NW), 1 (NE), 2(SW), 3(SE)
	int subFieldWidth;
	// Load the field & ------------------
	std::vector<Coi2> pos0_vec = { }; // in local (subfield) coordinates
	pos0_vec = LoadField4D("Resources/day18binput.txt", field, fieldWidth, nKeys, keyIndices, keyFieldCoordinates, subFields, subFieldWidth);

	// Initialize for Dijkstra ----------------
	
	std::set<std::string> visitedKeys;
	//std::queue<KeyPosition4D> keyQueue;
	
	//keyQueue.push(P0);
	std::map<std::string, CacheInfo4D> cache;
	DiagnosticData diagData;
	// End Init ==========================


	// Dijkstra
	//std::vector<std::pair<std::string, int>> result;
	KeyPosition4D P0 = { {{'1',pos0_vec[0]},{'2',pos0_vec[1]},{'3',pos0_vec[2]},{'4',pos0_vec[3]}},"@",0,"@" };
	KeyPosition4D Target = { {},"@abcdefghijklmno",0,"" };
	std::vector<KeyPosition4D> path = FindShortestPath(P0, Target, field, fieldWidth, visitedKeys, cache, diagData);
	std::cout << (path.end() - 1)->steps;

	//// Display Results:
	//std::cout << "\n\nResults found.\n";
	//std::cout << "==============\n";
	//std::cout << "Number of keys found: " << nKeys << '\n';
	//std::cout << "Number of unique routes: " << result.size() << '\n';

	//std::vector<std::pair<std::string, int>>::iterator max_it = std::max_element(result.begin(), result.end(), route_compare);
	//std::vector<std::pair<std::string, int>>::iterator min_it = std::min_element(result.begin(), result.end(), route_compare);
	//std::cout << "Range of route lengths found: [" << min_it->second << "..." << max_it->second << "]\n";
	//std::cout << "Example of shortest path: " << min_it->first << '\n';
	//std::cout << "Example of longest path : " << max_it->first << '\n';

	//std::cout << "\nAlgorithm diagnostics: \n";
	//std::cout << "====================== \n";
	//std::cout << "Outer BFS loop (key-to-key): \n";
	//std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_OuterBFS << '\n';
	//std::cout << "> Loop count: " << diagData.loopCount_OuterBFS << '\n';
	//std::cout << "Inner BFS loop (cell-to-cell): \n";
	//std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_InnerBFS << '\n';
	//std::cout << "> Loop count: " << diagData.loopCount_InnerBFS << '\n';
	//std::cout << "\nSize of key-to-key cache (#nodes in keyGraph): " << cache.size();

	////std::cout << "\n(press Enter to show keyGraph adjacency list)\n\n";
	////std::cin.get();
	//std::cout << "\n\nAdjacency list (Node name is keylocation + keys_owned)\n";
	//std::cout << "NODE                                 ARCS\n";
	//std::cout << "======================================================\n";
	//int count = 0;
	//for (auto e : cache)
	//{
	//	std::cout << std::left << std::setw(nKeys + 2) << e.first;
	//	std::cout << "->  ";
	//	for (auto p : e.second.cachedAvailablePositions)
	//	{
	//		if (p.size() > 0)
	//		{
	//			for (int i = 0; i < p.size(); i++)
	//			{
	//				std::cout << p[i].first.keychar << "[" << p[i].second << "], ";
	//			}
	//		}
	//	}
	//	std::cout << '\n';
	//	if (count == 15) break;
	//	count++;
	//}

	return;
}


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day18c();
	while (!_kbhit());
	return 0;
}


