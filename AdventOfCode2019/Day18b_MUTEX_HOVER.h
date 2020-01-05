#pragma once
#include "IncludesUsed.h"
std::string SerializeIntoString(Pos pos); // used to enable a hash lookup of visited nodes in the BFS
std::vector<Pos> LoadField(std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys);

struct KeyDependencies
{
	std::string globalKeys;
	int stepAccumulator = 0; // add # steps everytime a key is found by a position in the queue
	std::map<char, std::string> keyQuarter;
	std::map<char, std::string> doorQuarter;
	std::map<std::string, std::string> dependencyKeysMap;
};
std::string GetDependencyKeys(char door, KeyDependencies keyDependencies)
{
	std::string testString = keyDependencies.dependencyKeysMap[keyDependencies.doorQuarter[door]];
	auto indexOfDoor = testString.find(door);
	testString = testString.substr(0, indexOfDoor);
	std::string outputString;
	for (char c : testString)
	{
		if (c > 96 && c <= 122) outputString.push_back(c);
	}
	return outputString;
}
bool PositionOwnsAllDependencyKeys(const Pos& pos, std::string dependencyKeys)
{
	for (char c : dependencyKeys)
	{
		if (pos.keys.find(c) == std::string::npos)
		{
			return false;
		}
	}
	return true;
}
std::vector<Pos> FindNeighborsWithHovering(const std::vector<char>& field,
	const int& fieldWidth,
	Pos& pos,
	const std::set<std::string>& visited,
	KeyDependencies& keyDependencies)
{
	std::vector<Pos> availablePositions;
	int newX, newY;
	//for (int dir = 1; dir < 5; dir++)
	for (int dir = 4; dir > 0; dir--)
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
			newX = pos.x - 1;
			newY = pos.y;
		}
		else if (dir == 4) // EAST
		{
			newX = pos.x + 1;
			newY = pos.y;
		}

		char ch = field[newY * fieldWidth + newX];
		if (ch == '#') // If neighbor is [wall] don't include
		{
			// DISMISS
		}
		else if (ch > 64 && ch <= 90) // If neighbor is [door]
		{
			char door = ch;
			char doorkey = std::tolower(ch);
			// Update keyDependencies
			if (keyDependencies.doorQuarter.find(door) == keyDependencies.doorQuarter.end()) // door has not been opened before
			{
				if (keyDependencies.globalKeys.find(doorkey) != std::string::npos)	//(doorkey x is in globalkeys)
				{
					keyDependencies.doorQuarter[door] = pos.quarter; // add to doorQuarter map
					if (keyDependencies.keyQuarter[doorkey] != pos.quarter) // key was found in another quarter
					{
						keyDependencies.dependencyKeysMap[pos.quarter] += door;
						keyDependencies.dependencyKeysMap[keyDependencies.keyQuarter[doorkey]] += doorkey;
					}
				}
			}
			// Check if new position is available to this current postion instance (pos) that was taken off the BFS queue
			if (keyDependencies.globalKeys.find(doorkey) != std::string::npos)	//(doorkey x is in globalkeys)
			{
				std::string dependencyKeys = GetDependencyKeys(door, keyDependencies);
				if (keyDependencies.keyQuarter[doorkey] == pos.quarter) //(doorkey was found in own quarter)
				{
					if (pos.keys.find(doorkey) != std::string::npos) // pos owns own quarter's matching key)
					{
						// OPEN & PASS if not already visited
						Pos newpos = { newX,newY,pos.n + 1, pos.keys,pos.quarter };
						if (visited.find(SerializeIntoString(newpos)) == visited.end())
						{
							availablePositions.push_back(newpos);
						}
					}
					else
					{
						// DISMISS
					}
				}
				else if (PositionOwnsAllDependencyKeys(pos, dependencyKeys)) //position owns all the dependencykeys for door 
				{
					// OPEN & PASS if not already visited
					Pos newpos = { newX,newY,pos.n + 1, pos.keys,pos.quarter };
					if (visited.find(SerializeIntoString(newpos)) == visited.end())
					{
						availablePositions.push_back(newpos);
					}
				}
				else //(= > position does NOT owns all the dependencykeys for door X)
				{
					// DISMISS
				}
			}
			else //(=>doorkey x is not in globalkeys)
			{
				// HOVER
				availablePositions.push_back(pos);
				//####START DEBUG STATEMENT#####
				std::cout << "Hovering on " << pos.quarter + "_" + SerializeIntoString(pos) << " (dir checked: " << dir << "), nPos = " << pos.n << "\n";
				//####END DEBUG STATEMENT#####
			}
		}
		else if (ch > 96 && ch <= 122) //(neighbor is key) 
		{
			char key = ch;
			keyDependencies.keyQuarter[key] = pos.quarter;
			std::string newlocalkeys = pos.keys;

			if (pos.keys.find(key) == std::string::npos) // If new (local) key is found, add it to newpos.keys
			{
				newlocalkeys += key; // This will retain the order in which the keys are found. We sort when serializing
				//####START DEBUG STATEMENT#####
				//std::cout << "Local key found: " << ch <<", this bot now has keys "<<newkeys<<"\n" ; 
				//####END DEBUG STATEMENT#####
				if (keyDependencies.globalKeys.find(key) == std::string::npos) // if this key was not found before, add it to the keyDependencies struct
				{
					keyDependencies.globalKeys += key;
					keyDependencies.stepAccumulator += pos.n + 1;
					pos.n = -1; // reset step counter of the new position 
					//####START DEBUG STATEMENT#####
					std::cout << "Global key added: " << ch << ", globalKeys = " << keyDependencies.globalKeys << ", total steps: " << keyDependencies.stepAccumulator << "\n";
					//####END DEBUG STATEMENT#####
				}
			}
			Pos newpos = { newX,newY,pos.n + 1, newlocalkeys,pos.quarter };
			if (visited.find(SerializeIntoString(newpos)) == visited.end())
			{
				availablePositions.push_back(newpos);
			}
		}
		else // neighbor is free position
		{
			// PASS if not already visited
			Pos newpos = { newX,newY,pos.n + 1, pos.keys,pos.quarter };
			if (visited.find(SerializeIntoString(newpos)) == visited.end())
			{
				availablePositions.push_back(newpos);
			}
		}
	}
	return availablePositions;
}
int TimesInQueue(Pos p, std::queue<Pos> queue)
{
	int count = 0;
	while (!queue.empty())
	{
		Pos q = queue.front(); queue.pop();
		if (p == q) count++;
	}
	return count;
}
void Day18b_MUTEX_HOVER()
{
	// Initialize
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	KeyDependencies keyDependencies;
	std::vector<Pos> startPositions = LoadField("Resources/day18binput.txt", field, fieldWidth, nKeys);
	int maxQueueSize = 0;

	// Initialize queue and visited vector for BFS
	std::queue<Pos> queue;
	std::set<std::string> visited;
	for (auto s : startPositions)
	{
		queue.push(s);
		visited.insert(SerializeIntoString(s));
	}

	// BFS to find all keys
	while (!queue.empty())
	{
		maxQueueSize = max(maxQueueSize, queue.size());
		Pos currentpos = queue.front(); queue.pop();
		std::vector<Pos> neighbors = FindNeighborsWithHovering(field, fieldWidth, currentpos, visited, keyDependencies);
		for (Pos p : neighbors)
		{
			if (keyDependencies.globalKeys.size() == nKeys)
			{
				std::cout << "Last step to: (" << p.x << "," << p.y << "), keys = " << keyDependencies.globalKeys << std::endl;
				std::cout << "All keys obtained, " << keyDependencies.stepAccumulator << "steps taken.";
				while (!queue.empty()) queue.pop();
				break;
			}
			auto pstr = SerializeIntoString(p);
			if (visited.find(pstr) == visited.end())
			{
				visited.insert(pstr);
				//####START DEBUG STATEMENT#####
				//std::cout<<"New position added to visited set" << p.quarter + "_" + pstr << "\n";
				//####END DEBUG STATEMENT#####
			}
			//if (!(queue.back() == p))
			//if (TimesInQueue(p,queue)<2)
			{
				queue.push(p);
			}
		}
	}
	int k = 0;
}
