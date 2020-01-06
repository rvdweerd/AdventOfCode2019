#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
#include "Day18HelperFunctions.h"
#include "Day18_Old Working.h"

void Day18a()
{
	// Init =====================
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	Vei2 Pos0 = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys);
	PrintField(field, fieldWidth);
	std::set<std::string> visited;
	visited.insert(Hash(Pos0));
	std::queue<Vei2> queue;
	queue.push(Pos0);
	// END Init ==================

	// Main BFS loop
	std::string globalkeys;
	while (!queue.empty())
	{
		Vei2 curPos = queue.front(); queue.pop();
		std::vector<Vei2> newPositions = GetNewCoordinates(curPos,field, fieldWidth, visited);
		for (Vei2 newPos : newPositions)
		{
			// New status allocation
			newPos.steps++;
			char ch = field[newPos.coord[1] * fieldWidth + newPos.coord[0]];
			if (ch > 96 && ch <= 122 && (newPos.keys.find(ch) == std::string::npos)) // If new key is found, add it to keyring
			{
				newPos.keys += ch;
			}
			// visited mapping of new position/status
			visited.insert(Hash(newPos));
			// Place on queue
			queue.push(newPos);
			
			// Objective testing, break if reached
			if (newPos.keys.size() == nKeys)
			{
				std::cout << "Result reached. Number of steps: " << newPos.steps;
				while (!queue.empty()) queue.pop();
				break;
			}
		}
	}
}
void Day18b()
{
	// Init =====================
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::string globalKeys;
	Vei8 Pos0 = LoadField4C("Resources/day18binput.txt", field, fieldWidth, nKeys);
	PrintField(field, fieldWidth);
	std::set<std::string> visited;
	visited.insert(Hash4C(Pos0,globalKeys));
	std::queue<Vei8> queue;
	queue.push(Pos0);
	// END Init ==================

	// Main BFS loop
	bool mutexevent1 = false;
	bool mutexevent2 = false;
	int mutex = 0;
	while (!queue.empty())
	{
		Vei8 curPos = queue.front(); queue.pop();
		if (mutexevent1 || mutexevent2)
		{
			mutex = (mutex+1) % 4;
			if (mutexevent1) mutexevent1 = false;
			if (mutexevent2) mutexevent2 = false;
		}
		std::vector<Vei8> newPositions = GetNewCoordinates4C(curPos, field, fieldWidth, visited, globalKeys, mutex);
		if (newPositions.size() == 0)
		{
			mutexevent1 = true;
			queue.push(curPos);
		}
		for (Vei8 newPos : newPositions)
		{
			// New status allocation
			newPos.steps++;
			for (size_t i = 0; i < 4; i++)
			{
				char ch = field[newPos.coord[2*i+1] * fieldWidth + newPos.coord[2*i]];
				if (ch > 96 && ch <= 122 && (globalKeys.find(ch) == std::string::npos)) // If new key is found, add it to keyring
				{
					newPos.keys += ch;
					mutexevent2 = true;
					if (globalKeys.find(ch) == std::string::npos)
					{
						globalKeys += ch;
						std::cout << ch;
					}
				}
			}
			// visited mapping of new position/status
			std::string hash = Hash4C(newPos,globalKeys);
			visited.insert(hash);
			std::cout << hash<<", n="<<newPos.steps<<'\n';
			
			// Place on queue
			queue.push(newPos);

			// Objective testing, break if reached
			if (newPos.keys.size() == nKeys)
			{
				std::cout << "Result reached. Number of steps: " << newPos.steps;
				while (!queue.empty()) queue.pop();
				break;
			}
		}
	}
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

