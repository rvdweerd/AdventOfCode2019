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
	KeyPosition P0 = { '@',"@",0, keyFieldCoordinates['@'], "@"};
	std::set<std::string> visitedKeys;
	std::queue<KeyPosition> keyQueue;
	keyQueue.push(P0);
	std::map<std::string, std::vector<std::pair<Key, int>>> cache;
	// End Init ==========================

	// Main BFS loop
	std::vector<std::pair<std::string, int>> result;
	while (!keyQueue.empty())
	{
		KeyPosition curKeyPos = keyQueue.front(); keyQueue.pop();
		std::vector<std::pair<Key, int>> newKeys = GetAvailableKeyPositions(curKeyPos, field, fieldWidth, visitedKeys, cache);
		
		std::vector<KeyPosition> newPositions;
		for (std::pair<Key, int> p : newKeys)
		{
			std::string newkeys = curKeyPos.keys + p.first.key;
			std::sort(newkeys.begin(), newkeys.end());
			KeyPosition newKeyPosition = { p.first.key , newkeys, curKeyPos.steps + p.second,p.first.coordinates,curKeyPos.path + p.first.key };
			newPositions.push_back(newKeyPosition);
		}

		for (KeyPosition p : newPositions)
		{
			if (p.keys.size() == nKeys) // end condition
			{
				std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
				result.push_back({ p.path,p.steps });
			}
			else
			{
				visitedKeys.insert(Hash(p));
				keyQueue.push(p);
			}
		}
	}
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

