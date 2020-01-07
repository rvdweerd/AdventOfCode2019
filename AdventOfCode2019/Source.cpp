#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
#include "Day18BHelperFunctions.h"

//void Day18a()
//{
//	// Init global data structs===========
//	std::vector<char> field;
//	int fieldWidth;
//	int nKeys;
//	std::map<char, int> keyIndices;
//	std::map<char, Coi2> keyFieldCoordinates;
//	// Load the field & ------------------
//	Coi2 pos0 = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys, keyIndices, keyFieldCoordinates);
//	PrintField(field, fieldWidth);
//	// Initialize for BFS ----------------
//	KeyPosition P0 = { {'@',keyFieldCoordinates['@']},"@",0,"@" };
//	std::set<std::string> visitedKeys;
//	std::queue<KeyPosition> keyQueue;
//	keyQueue.push(P0);
//	std::map<std::string,CacheInfo> cache;
//	DiagnosticData diagData;
//	// End Init ==========================
//
//	// Main BFS loop
//	std::vector<std::pair<std::string, int>> result;
//	while (!keyQueue.empty())
//	{
//		if (keyQueue.size() > diagData.MaxQueueSize_OuterBFS) diagData.MaxQueueSize_OuterBFS = keyQueue.size();
//		diagData.loopCount_OuterBFS++;
//		KeyPosition curKeyPos = keyQueue.front(); keyQueue.pop();
//		std::vector<std::pair<Key, int>> newKeys = GetAvailableKeyPositions(curKeyPos, field, fieldWidth, visitedKeys, cache, diagData);
//		
//		std::vector<KeyPosition> newPositions;
//		for (std::pair<Key, int> p : newKeys)
//		{
//			std::string newkeys = curKeyPos.keys + p.first.keychar;
//			std::sort(newkeys.begin(), newkeys.end());
//			KeyPosition newKeyPosition = { p.first , newkeys, curKeyPos.steps + p.second, curKeyPos.path + p.first.keychar };
//			newPositions.push_back(newKeyPosition);
//		}
//
//		for (KeyPosition p : newPositions)
//		{
//			if (p.keys.size() == nKeys) // end condition
//			{
//				//std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
//				result.push_back({ p.path,p.steps });
//			}
//			else
//			{
//				visitedKeys.insert(Hash(p));
//				keyQueue.push(p);
//			}
//		}
//	}
//
//	// Display Results:
//	std::cout << "\n\nResults found.\n";
//	std::cout << "==============\n";
//	std::cout << "Number of keys found: " << nKeys << '\n';
//	std::cout << "Number of unique routes: " << result.size() << '\n';
//	
//	std::vector<std::pair<std::string, int>>::iterator max_it = std::max_element(result.begin(), result.end(), route_compare);
//	std::vector<std::pair<std::string, int>>::iterator min_it = std::min_element(result.begin(), result.end(), route_compare);
//	std::cout << "Range of route lengths found: [" << min_it->second << "..." << max_it->second<<"]\n";
//	std::cout << "Example of shortest path: " << min_it->first << '\n';
//	std::cout << "Example of longest path : " << max_it->first << '\n';
//
//	std::cout << "\nAlgorithm diagnostics: \n";
//	std::cout << "====================== \n";
//	std::cout << "Outer BFS loop (key-to-key): \n";
//	std::cout << "> Maximum queue size: "<< diagData.MaxQueueSize_OuterBFS<< '\n';
//	std::cout << "> Loop count: " << diagData.loopCount_OuterBFS<<'\n';
//	std::cout << "Inner BFS loop (cell-to-cell): \n";
//	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_InnerBFS<< '\n';
//	std::cout << "> Loop count: " << diagData.loopCount_InnerBFS<<'\n';
//	std::cout << "\nSize of key-to-key cache (#nodes in keyGraph): " << cache.size();
//	
//	//std::cout << "\n(press Enter to show keyGraph adjacency list)\n\n";
//	//std::cin.get();
//	std::cout << "\n\nAdjacency list (Node name is keylocation + keys_owned)\n";
//	std::cout << "NODE         ARCS\n";
//	std::cout << "======================================================\n";
//	for (auto e : cache)
//	{
//		std::cout << std::left<< std::setw(nKeys+2) << e.first;
//		std::cout << "->  ";
//		for (auto p : e.second.cachedAvailablePositions)
//		{
//			std::cout << p.first.keychar << "[" << p.second << "], ";
//		}
//		std::cout << '\n';
//	}
//	return;
//}
void Day18b()
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
	
	// Initialize for BFS ----------------
	KeyPosition4D P0 = { {{'1',pos0_vec[0]},{'2',pos0_vec[1]},{'3',pos0_vec[2]},{'4',pos0_vec[3]}},"@",0,"@" };
	std::set<std::string> visitedKeys;
	std::queue<KeyPosition4D> keyQueue;
	keyQueue.push(P0);
	std::map<std::string, CacheInfo4D> cache;
	DiagnosticData diagData;
	// End Init ==========================


	// Main BFS loop
	std::vector<std::pair<std::string, int>> result;
	while (!keyQueue.empty())
	{
		if (keyQueue.size() > diagData.MaxQueueSize_OuterBFS) diagData.MaxQueueSize_OuterBFS = keyQueue.size();
		diagData.loopCount_OuterBFS++;
		KeyPosition4D curKeyPos = keyQueue.front(); keyQueue.pop();
		std::vector<std::vector<std::pair<Key, int>>> newKeys = GetAvailableKeyPositionsForSubFields(curKeyPos, field, fieldWidth, visitedKeys, cache, diagData);
		
		if (!newKeys.size() == 0)
		{
			std::vector<KeyPosition4D> newPositions;
			for (int i = 0; i < 4; i++) // loop the 4 subFields // for (std::vector<std::pair<Key, int>> pVec : newKeys)
			{
				std::vector<std::pair<Key, int>> pVec = newKeys[i];
				for (std::pair<Key, int> p : pVec)
				{
					KeyPosition4D newKeyPosition = curKeyPos;
					newKeyPosition.key[i] = p.first;
					newKeyPosition.steps += p.second;
					std::string newkeys = curKeyPos.keys + p.first.keychar;
					std::sort(newkeys.begin(), newkeys.end());
					newKeyPosition.keys = newkeys;
					newKeyPosition.path += p.first.keychar;
					newPositions.push_back(newKeyPosition);
				}
			}
			for (KeyPosition4D pos : newPositions)
			{
				if (pos.keys.size() == nKeys) // end condition
				{
					//std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
					result.push_back({ pos.path,pos.steps });
				}
				else
				{
					//visitedKeys.insert(Hash(p));
					keyQueue.push(pos);
				}
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
	std::cout << "NODE                                 ARCS\n";
	std::cout << "======================================================\n";
	int count = 0;
	for (auto e : cache)
	{
		std::cout << std::left<< std::setw(nKeys+2) << e.first;
		std::cout << "->  ";
		for (auto p : e.second.cachedAvailablePositions)
		{
			if (p.size() > 0)
			{
				for (int i = 0; i < p.size(); i++)
				{
					std::cout << p[i].first.keychar <<  "[" << p[i].second << "], ";
				}
			}
		}
		std::cout << '\n';
		if (count == 15) break;
		count++;
	}

	return;
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day18b();
	
	while (!_kbhit());
	return 0;
}

