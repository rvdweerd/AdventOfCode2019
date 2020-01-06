/*
#pragma once
#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
namespace d18old
{
	void PrintField(std::vector<char> field, const int& fieldWidth)
	{
		//std::cout << '\n';
		for (size_t i = 0; i < field.size(); i++)
		{
			if (i % fieldWidth == 0) std::cout << '\n';
			if ((i > 2 * fieldWidth && (i < field.size() - 2 * fieldWidth)) && //away from top/bottom
				(field[i - fieldWidth] == '.' || field[i - fieldWidth] > 65) &&
				(field[i + fieldWidth] == '.' || field[i + fieldWidth] > 65) && // above and below is corridor
				(field[i] == '#')) // cell is barrier
			{
				std::cout << '-';
			}
			else if (field[i - 1] == '.' && field[i + 1] == '.' && field[i] == '#')
			{
				std::cout << '|';
			}
			else if (field[i] == '#')
			{
				std::cout << '+';
			}
			else if (field[i] == '.')
			{
				std::cout << ' ';
			}
			else
			{
				std::cout << field[i];
			}
		}
		std::cout << "\n";
	}
	void DebugPrint(std::vector<char> field, const int& fieldWidth, const Pos& pos)
	{
		for (char& c : field)
		{
			if (c == '@') c = '.';
		}
		field[pos.y * fieldWidth + pos.x] = '@';
		PrintField(field, fieldWidth);
	}
	void DebugPrint(std::vector<char> field, const int& fieldWidth, const std::vector<Pos>& startPositions)
	{
		for (char& c : field)
		{
			if (c == '@') c = '.';
		}
		for (Pos p : startPositions)
		{
			field[p.y * fieldWidth + p.x] = '@';
		}
		PrintField(field, fieldWidth);
	}
	//void DebugPrintWithActiveQueue(std::vector<char> field, const int& fieldWidth, const std::vector<Pos>& pos, std::queue<std::vector<Pos>> queue)
	//{
	//	for (char& c : field)
	//	{
	//		if (c == '@') c = '.';
	//	}
	//	for (Pos p : pos)
	//	{
	//		field[p.y * fieldWidth + p.x] = '@';
	//	}
	//	while (!queue.empty())
	//	{
	//		std::vector<Pos> v = queue.front(); queue.pop();
	//		for (Pos p : v)
	//		{
	//			field[p.y * fieldWidth + p.x] = '$';
	//		}
	//	}
	//	PrintField(field, fieldWidth);
	//}
	std::string SerializeIntoString(Pos pos) // used to enable a hash lookup of visited nodes in the BFS
	{
		std::sort(pos.keys.begin(), pos.keys.end());
		pos.keys.append(std::to_string(pos.x));
		pos.keys.append("_");
		pos.keys.append(std::to_string(pos.y));
		return pos.keys;
	}
	std::vector<Pos> FindNeighborsNoHovering(const std::vector<char>& field, const int& fieldWidth, const Pos& pos, const std::set<std::string>& visited)
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
				newX = pos.x - 1;
				newY = pos.y;
			}
			else if (dir == 4) // EAST
			{
				newX = pos.x + 1;
				newY = pos.y;
			}

			char ch = field[newY * fieldWidth + newX];
			if ((ch == '#') || // If neighbor is [wall] or [door without key] don't include
				(ch > 64 && ch <= 90 && (pos.keys.find(std::tolower(ch)) == std::string::npos)))
			{
			}
			else // Else, include neighbor, if not already visited, add key if found
			{
				std::string newkeys = pos.keys;
				if (ch > 96 && ch <= 122 && (newkeys.find(ch) == std::string::npos)) // If new key is found, add it to newpos.keys
				{
					newkeys += ch; // This will retain the order in which the keys are found. We sort when serializing
				}
				Pos newpos = { newX,newY,pos.n + 1, newkeys };
				if (visited.find(SerializeIntoString(newpos)) == visited.end())
				{
					availablePositions.push_back(newpos);
				}
			}
		}
		return availablePositions;
	}
	std::vector<Pos> LoadField(std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys)
	{
		fieldWidth = 1; // we don't know this yet, this is the starting assumption
		bool fieldWidthSet = false;
		std::ifstream in(filename);
		int countStartPositions = 0;
		std::vector<Pos> startPositions;
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
						Pos startpos;
						startpos.y = i / fieldWidth;
						startpos.x = i - startpos.y * fieldWidth;
						//startpos.quarter = std::string("Q") + std::to_string(countStartPositions + 1);
						startpos.quarter = "Q" + std::to_string(countStartPositions + 1);
						countStartPositions++;
						startPositions.push_back(startpos);
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
		return startPositions;
	}
	void Day18a()
	{
		bool DEBUG = false;
		// Initialize
		std::vector<char> field;
		int fieldWidth;
		int nKeys;
		std::vector<Pos> startPositions = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys);
		if (startPositions.size() != 1) std::cout << "Error loading field.";
		Pos startpos = startPositions[0];

		if (DEBUG)
		{
			std::cout << "STARTING GRID\n";
			DebugPrint(field, fieldWidth, startpos);
			std::cout << "\nMOVEMENT TRACKING\n";
		}

		// BFS to find all keys
		std::queue<Pos> queue;
		std::set<std::string> visited;
		queue.push(startpos);
		visited.insert(SerializeIntoString(startpos));

		while (!queue.empty())
		{
			Pos currentpos = queue.front(); queue.pop();
			std::vector<Pos> neighbors = FindNeighborsNoHovering(field, fieldWidth, currentpos, visited);

			if (DEBUG)
			{
				std::cout << "\nMain loop. \n==========\nFront queue position taken. ";
				std::cout << "New queue size: " << queue.size() << '\n';
				std::cout << "Current position investigated: (" << currentpos.x << "," << currentpos.y << "). Cumulative steps: " << currentpos.n << ".\n";
				if (neighbors.size() > 0)
				{
					std::cout << "Found " << neighbors.size() << " accessible neighbors: ";
					for (Pos p : neighbors)
					{
						std::cout << "(" << p.x << ", " << p.y << "); ";
					}
					std::cout << neighbors.size() << " positions added to the queue.\n";
				}
				else
				{
					std::cout << "Found no accessible neighbors. ";
					std::cout << "Nothing added to the queue.\n";
				}
				std::cout << "Current key string: " << currentpos.keys << '\n';
				//DebugPrintWithActiveQueue(field, fieldWidth, currentpos, queue, );
			}

			for (Pos p : neighbors)
			{
				if (p.keys.size() == nKeys)
				{
					std::cout << "Last step to: (" << p.x << "," << p.y << "), keys = " << p.keys << std::endl;
					std::cout << "All keys obtained, " << p.n << " steps taken.";
					while (!queue.empty()) queue.pop();
					break;
				}
				visited.insert(SerializeIntoString(p));
				queue.push(p);

				if (DEBUG)
				{
					std::cout << "Adding serialized position to the visited set: " << SerializeIntoString(p);
					std::cout << "; New queue size: " << queue.size() << '\n';
				}
			}
		}
	}

	struct KeyDependencies
	{
		std::string globalKeys;
		int stepAccumulator = 0; // add # steps everytime a key is found by a position in the queue
		std::map<char, std::string> keyQuarter;
		std::map<char, std::string> doorQuarter;
		std::map<std::string, std::string> dependencyKeysMap;
		std::map<std::string, std::pair<int, int>> keyCountMap;
		std::map<std::string, std::string> keyRingMap;
		std::map<std::string, std::string> keyPathwayMap;
	};
	void DebugPrintWithActiveQueue(std::vector<char> field, const int& fieldWidth, const Pos& pos, std::queue<Pos> queue, KeyDependencies& keyDependencies)
	{

		for (char& c : field)
		{
			if (c == '@') c = '.';
		}
		field[pos.y * fieldWidth + pos.x] = '@';
		std::cout << "\nAll queue positions ($):\n";
		while (!queue.empty())
		{
			Pos p = queue.front(); queue.pop();
			int checksum = keyDependencies.keyCountMap[p.quarter].first;
			if (p.keys.size() == checksum)
			{
				field[p.y * fieldWidth + p.x] = '>';
			}
			else
			{
				field[p.y * fieldWidth + p.x] = '$';
			}
			//std::cout << "(" << p.x << "," << p.y << ")n("<< p.n <<"), ";
		}
		//std::cout << "\nField with current (@) and all queue ($) positions:\n";
		PrintField(field, fieldWidth); std::cout << "\n";
	}

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
	std::string GetDependencyKeysForAKey(char key, KeyDependencies keyDependencies)
	{
		std::string testString = keyDependencies.dependencyKeysMap[keyDependencies.keyQuarter[key]];
		auto indexOfKey = testString.find(key);
		testString = testString.substr(0, indexOfKey);
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
	std::string SerializeIntoStringB(Pos pos, const KeyDependencies& keyDependencies) // used to enable a hash lookup of visited nodes in the BFS
	{
		std::string outString_G = keyDependencies.globalKeys;
		std::sort(outString_G.begin(), outString_G.end());
		std::string outString_L = pos.keys;
		std::sort(outString_L.begin(), outString_L.end());
		std::string outString = "[" + outString_L + "_" + outString_G + "](";
		outString.append(std::to_string(pos.x));
		outString.append(",");
		outString.append(std::to_string(pos.y));
		outString.append(")");
		return outString;
	}

	std::vector<Pos> FindNeighborsWithHovering(
		const std::vector<char>& field,
		const int& fieldWidth,
		Pos& pos,
		std::set<std::string>& visited,
		KeyDependencies& keyDependencies,
		bool& DEBUG)
	{
		KeyDependencies keyDependencies_orig = keyDependencies;
		Pos pos_orig = pos;
		if (DEBUG)
		{
			std::cin.get();
			ClearScreen();
			std::cout << "Finding neighbors for: (" << pos.x << "," << pos.y << '\n';
		}
		std::vector<Pos> availablePositions;
		int newX, newY;
		int countReset = 0;
		int doorHoveringCount = 0;
		std::vector<int> dirorder = { 4,3,2,1 };
		//for (int dir = 1; dir < 5; dir++)
		//for (int dir = 4; dir > 0; dir--)
		for (auto dirIterator = 0; dirIterator < 4; dirIterator++)
		{
			int dir = dirorder[dirIterator];
			if (dir == 1) { newX = pos.x;	newY = pos.y - 1; }
			else if (dir == 2) { newX = pos.x;	newY = pos.y + 1; }
			else if (dir == 3) { newX = pos.x - 1; newY = pos.y; }
			else if (dir == 4) { newX = pos.x + 1; newY = pos.y; }

			char ch = field[newY * fieldWidth + newX];
			if (ch == '#') // If neighbor is [wall] don't include
			{
				if (DEBUG)
				{
					std::cout << "Dir " << dir << ", wall detected\n";
				}
				// DISMISS
			}
			else if (ch > 64 && ch <= 90) // If neighbor is [door]
			{
				if (DEBUG)
				{
					std::cout << "Dir " << dir << ", door detected. ";
				}
				if (false) // Make sure that there is always one positiong hovering at a door no matter what
				{
					if (doorHoveringCount < 1) // avoid unneccesary duplication 
					{
						// HOVER
						//pos.path = ch; 
						availablePositions.push_back(pos);
						doorHoveringCount++;
						//####START DEBUG STATEMENT#####
						if (DEBUG)
						{
							std::cout << "Door HOVER enforced. ";
							std::cout << "Pos " << pos.quarter + ": " + SerializeIntoStringB(pos, keyDependencies_orig) << "), nPos = " << pos.n << "\n";
						}
						//####END DEBUG STATEMENT#####
					}
				}
				char door = ch;
				char doorkey = std::tolower(ch);
				// Update keyDependencies
				if (keyDependencies_orig.doorQuarter.find(door) == keyDependencies_orig.doorQuarter.end()) // door has not been opened before
				{
					if (keyDependencies_orig.globalKeys.find(doorkey) != std::string::npos)	//(doorkey x is in globalkeys)
					{
						keyDependencies.doorQuarter[door] = pos_orig.quarter; // add to doorQuarter map
						if (keyDependencies_orig.keyQuarter[doorkey] != pos_orig.quarter) // key was found in another quarter
						{
							keyDependencies.dependencyKeysMap[pos_orig.quarter] += door;
							keyDependencies.dependencyKeysMap[keyDependencies.keyQuarter[doorkey]] += doorkey;
							keyDependencies_orig.dependencyKeysMap[pos_orig.quarter] += door;
							keyDependencies_orig.dependencyKeysMap[keyDependencies.keyQuarter[doorkey]] += doorkey;
							assert(!StringHasDuplicateCharacters(keyDependencies.dependencyKeysMap[keyDependencies.keyQuarter[doorkey]]));
							assert(!StringHasDuplicateCharacters(keyDependencies.dependencyKeysMap[pos_orig.quarter]));
							assert(!StringHasDuplicateCharacters(keyDependencies_orig.dependencyKeysMap[keyDependencies.keyQuarter[doorkey]]));
							assert(!StringHasDuplicateCharacters(keyDependencies_orig.dependencyKeysMap[pos_orig.quarter]));
						}
					}
				}
				// Check if new position (door) is available to this current postion instance (pos) that was taken off the BFS queue
				if (keyDependencies_orig.globalKeys.find(doorkey) != std::string::npos)	//(doorkey x is in globalkeys)
				{
					std::string dependencyKeys = GetDependencyKeys(door, keyDependencies);
					if (keyDependencies_orig.keyQuarter[doorkey] == pos_orig.quarter) //(doorkey was found in own quarter)
					{
						if (pos_orig.keys.find(doorkey) != std::string::npos) // pos owns own quarter's matching key)
						{
							if (DEBUG)
							{
								std::cout << "Can open with local key, owns all local dep-keys and local key:";
							}
							// OPEN & PASS if not already visited
							Pos newpos = { newX,newY,pos.n + 1, pos_orig.keys,pos.quarter,pos.path + ch };
							if (visited.find(SerializeIntoStringB(newpos, keyDependencies_orig)) == visited.end())
							{
								if (DEBUG)
								{
									std::cout << "->PASS (not visited earlier in this status)\n";
								}
								availablePositions.push_back(newpos);
							}
							else
							{
								if (DEBUG)
								{
									std::cout << "->DISMISS (visited earlier in this status)\n";
								}
							}
						}
						else
						{
							if (DEBUG)
							{
								std::cout << "Position Dismissed, doen'st have dependency keys and/or local key\n";
							}
							// DISMISS
						}
					}
					else if (PositionOwnsAllDependencyKeys(pos, dependencyKeys)) //position owns all the dependencykeys for door 
					{
						if (DEBUG)
						{
							std::cout << "Can open with global key AND owns all local dep-keys ";
						}
						// OPEN & PASS if not already visited
						Pos newpos = { newX,newY,pos.n + 1, pos_orig.keys,pos_orig.quarter, pos_orig.path + ch };
						if (visited.find(SerializeIntoStringB(newpos, keyDependencies_orig)) == visited.end())
						{
							if (DEBUG)
							{
								std::cout << "->PASS, because not visited earlier\n";
							}
							availablePositions.push_back(newpos);
						}
						else
						{
							if (DEBUG)
							{
								std::cout << "->DISMISS, new position has been visited earlier in this status\n";
							}
						}
					}
					else //(= > position does NOT owns all the dependencykeys for door X)
					{
						if (DEBUG)
						{
							std::cout << "DISMISS, position does not own all the dependency keys for door.\n";
						}
						// DISMISS
					}
				}
				else if (doorHoveringCount < 1)//(=>doorkey x is not in globalkeys)
				{
					// HOVER
					availablePositions.push_back(pos);
					doorHoveringCount++;
					//####START DEBUG STATEMENT#####
					if (DEBUG)
					{
						if (DEBUG)
						{
							std::cout << "HOVERING, no global key yet. ";
							std::cout << "Pos " << pos.quarter + ": " + SerializeIntoStringB(pos, keyDependencies_orig) << "), nPos = " << pos.n << "\n";
						}
					}
					//####END DEBUG STATEMENT#####
				}
				else // avoid duplicate hovering (and exploding the queue)
				{
					if (DEBUG)
					{
						std::cout << "DISMISS, already one position hovering.\n";
					}
				}
			}
			else if (ch > 96 && ch <= 122) //(neighbor is key) 
			{
				if (DEBUG)
				{
					std::cout << "Dir " << dir << ", key detected. ";
				}
				char key = ch;
				keyDependencies.keyQuarter[key] = pos_orig.quarter;
				std::string newlocalkeys = pos_orig.keys;

				if (pos_orig.keys.find(key) == std::string::npos) // If new (local) key is found, add it to newpos.keys
				{
					newlocalkeys += key; // This will retain the order in which the keys are found. We sort when serializing
					//####START DEBUG STATEMENT#####
					if (DEBUG)
					{
						std::cout << "Local key added [" << ch << "] to (" << newlocalkeys << ").";
					}
					//####END DEBUG STATEMENT#####
					if (keyDependencies_orig.globalKeys.find(key) == std::string::npos) // if this key was not found before, add it to the keyDependencies struct
					{
						keyDependencies.globalKeys += key;
						keyDependencies.keyCountMap[pos.quarter] = { newlocalkeys.size() , pos_orig.n + 1 };
						keyDependencies.keyRingMap[pos.quarter] = newlocalkeys;
						keyDependencies.keyPathwayMap[pos.quarter] = pos_orig.path + key;
						assert(!StringHasDuplicateCharacters(keyDependencies.globalKeys));
						keyDependencies.stepAccumulator += pos.n + 1;
						countReset = pos.n + 1; // reset step counter of the new position 
						//####START DEBUG STATEMENT#####
						if (DEBUG)
						{
							std::cout << "Fist global find, added [" << ch << "] to (" << keyDependencies.globalKeys << "), add steps: " << countReset << ", total steps: " << keyDependencies.stepAccumulator;
						}
						//####END DEBUG STATEMENT#####
					}
					else // it's new to you (but not to global), update key accounting if you have more keys or were quicker than the previous finder
					{
						assert(!StringHasDuplicateCharacters(newlocalkeys));
						if (newlocalkeys.size() > keyDependencies.keyCountMap[pos.quarter].first)//||
								//pos_orig.n + 1 < keyDependencies.keyCountMap[pos.quarter].second )
						{
							//
							{
								keyDependencies.keyCountMap[pos.quarter] = { newlocalkeys.size() , pos_orig.n + 1 };
								keyDependencies.keyRingMap[pos.quarter] = newlocalkeys;
								keyDependencies.keyPathwayMap[pos.quarter] = pos_orig.path + key;
							}
						}
					}
				}
				else
				{
					if (DEBUG)
					{
						std::cout << "Key already found, not added.";
					}
				}
				//Pos newpos = { newX,newY,pos.n + 1-countReset, newlocalkeys,pos_orig.quarter };
				Pos newpos = { newX,newY,pos.n + 1, newlocalkeys,pos_orig.quarter , pos_orig.path + ch };
				if (visited.find(SerializeIntoStringB(newpos, keyDependencies_orig)) == visited.end())
				{
					if (DEBUG)
					{
						std::cout << "->PASS, not visited earlier\n";
					}
					availablePositions.push_back(newpos);
					visited.insert(SerializeIntoStringB(newpos, keyDependencies));
				}
				else
				{
					if (DEBUG)
					{
						std::cout << "->DISMISS, visited earlier in this status.\n";
					}
				}
			}
			else // neighbor is free position
			{
				if (DEBUG)
				{
					std::cout << "Dir " << dir << ", free passage, ";
				}
				// PASS if not already visited
				Pos newpos = { newX,newY,pos.n + 1, pos.keys,pos.quarter,pos.path };
				if (visited.find(SerializeIntoStringB(newpos, keyDependencies_orig)) == visited.end())
				{
					if (DEBUG)
					{
						std::cout << " not visited earlier in this setup so pass.\n";
					}
					availablePositions.push_back(newpos);
				}
				else
				{
					if (DEBUG)
					{
						std::cout << " but visited earlier so Dismissing.\n";
					}
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
	void Day18b()
	{
		bool DEBUG = false;

		// Initialize
		std::vector<char> field;
		int fieldWidth;
		int nKeys;
		KeyDependencies keyDependencies;
		std::vector<Pos> startPositions = LoadField("Resources/day18binput.txt", field, fieldWidth, nKeys);
		int maxQueueSize = 0;
		int loopcount = 0;


		// Initialize queue and visited vector for BFS
		std::queue<Pos> queue;
		std::set<std::string> visited;
		for (auto s : startPositions)
		{
			queue.push(s);
			visited.insert(SerializeIntoStringB(s, keyDependencies));
		}

		if (DEBUG)
		{
			std::cout << "STARTING GRID\n";
			DebugPrint(field, fieldWidth, startPositions);
			std::cout << "\nSTART MOVEMENT TRACKING (press enter)\n";
		}
		// BFS to find all keys
		while (!queue.empty())
		{

			maxQueueSize = max(maxQueueSize, queue.size());
			loopcount++;
			Pos currentpos = queue.front(); queue.pop();

			if (loopcount % 10000 == 0)
			{
				ClearScreen();
				DebugPrintWithActiveQueue(field, fieldWidth, currentpos, queue, keyDependencies);
				std::cout << "\n global keys: " << keyDependencies.globalKeys << "(" << keyDependencies.globalKeys.size() << ")";
				std::cout << "\n queue size before cleanup: " << queue.size();
				std::cout << "\n size of visited before cleanup: " << visited.size();

				//std::vector<Pos> vec;
				//while (queue.size()>0)
				//{
				//	Pos p = queue.front();
				//	bool alreadyContains = false;
				//	for (size_t i=0;i<vec.size();i++)
				//	{
				//		Pos pTest = vec[i];
				//		if ((p.x == pTest.x) && (p.y == pTest.y) && (p.keys.size() > pTest.keys.size()))
				//		{
				//			//if (p.n < pTest.n)
				//			{
				//				vec[i] = p;
				//			}
				//			alreadyContains = true;
				//		}
				//	}
				//	if (!alreadyContains)
				//	{
				//		vec.push_back(p);
				//	}
				//	queue.pop();
				//}
				//for (Pos p : vec)
				//{
				//	queue.push(p);
				//}
				//std::cout << "\n queue size after cleanup: " << queue.size();
				//int k = 0; std::cout << "";
				if (visited.size() > 5e6)
				{
					for (auto it = visited.begin(); it != visited.end();) {
						if ((*it).size() < (keyDependencies.globalKeys.size() + 8))
							it = visited.erase(it);
						else ++it;
					}
				}
			}
			//while (queue.size() > 0)
			//{
			//	Pos nextpos = queue.front();
			//	if (
			//		nextpos.x == currentpos.x && nextpos.y == currentpos.y &&
			//		nextpos.keys == currentpos.keys &&
			//		nextpos.n <= currentpos.n )
			//	{
			//		currentpos = nextpos; queue.pop();
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}

			std::vector<Pos> neighbors = FindNeighborsWithHovering(field, fieldWidth, currentpos, visited, keyDependencies, DEBUG);

			if (DEBUG)
			{
				DebugPrint(field, fieldWidth, currentpos);
				std::cout << "\nMain loop. \n==========\nFront queue position taken. ";
				std::cout << "New queue size: " << queue.size() << '\n';
				std::cout << "Current positions investigated:\n";
				//for (Pos p : currentPositions)
				{
					std::cout << "(" << currentpos.x << ", " << currentpos.y << "),n=" << currentpos.n << ", " << currentpos.quarter << ", pos.keys=" << currentpos.keys << "\n";
				}

				if (neighbors.size() > 0)
				{
					std::cout << "Found " << neighbors.size() << " accessible neighbor positions:\n";
					//for (std::vector<Pos> v : neighborVecs)
					//{
					for (Pos p : neighbors)
					{
						std::cout << "(" << p.x << ", " << p.y << "),n=" << p.n << ", " << p.quarter << ", pos.keys=" << p.keys << "\n";
					}
					std::cout << neighbors.size() << " new position vectors added to the queue.\n";
					std::cout << "------------------------------\n";
					//}

				}
				else
				{
					std::cout << "Found no accessible neighbors. ";
					std::cout << "Nothing added to the queue.\n";
				}

				std::cout << "Current global key string: " << keyDependencies.globalKeys << '\n';
				std::cout << "Current status of queue: \n(<--front    back-->)\n";
				DebugPrintWithActiveQueue(field, fieldWidth, currentpos, queue, keyDependencies);
			}
			for (Pos p : neighbors)
			{
				int keycheck = 0;
				for (auto e : keyDependencies.keyCountMap)
				{
					std::pair<int, int> pair = e.second;
					keycheck += pair.first;
				}
				if (keycheck == nKeys)
				{
					if (DEBUG) ClearScreen();
					std::cout << "Last step to: (" << p.x << "," << p.y << "), all keys obtained. \n";
					std::cout << "\nOperation required : \n";
					std::cout << "> Main loop iterations: " << loopcount << '\n';
					std::cout << "> Maxumum queue size:   " << maxQueueSize << '\n';

					std::cout << "\nKeyring map:\n";
					for (auto a : keyDependencies.keyRingMap)
					{
						std::cout << "- Quarter = " << a.first << ", keyring = " << a.second;
						std::cout << " (" << keyDependencies.keyCountMap[a.first].first << " keys), ";
						std::cout << "minimum steps : " << keyDependencies.keyCountMap[a.first].second << '\n';
					}
					std::cout << "\nPathway map:\n";
					for (auto a : keyDependencies.keyPathwayMap)
					{
						std::cout << "- Quarter = " << a.first << ", pathway = " << a.second << '\n';
					}
					int stepcount = 0;
					for (auto e : keyDependencies.keyCountMap)
					{
						std::pair<int, int> pair = e.second;
						stepcount += pair.second;
					}
					std::cout << "\nResults:\n";
					std::cout << "> Minimum steps to find all keys: " << stepcount << '\n';
					std::cout << "> Global keypath  : " << keyDependencies.globalKeys << " (note, this is order of new global keys added, not the winning route." << '\n';

					while (!queue.empty()) queue.pop();
					break;
				}
				auto pstr = SerializeIntoStringB(p, keyDependencies);
				if (visited.find(pstr) == visited.end())
				{
					visited.insert(pstr);
					//####START DEBUG STATEMENT#####  
					if (DEBUG)
					{
						std::cout << "New position from " << p.quarter << " added to visited set: " << pstr << "\n";

						//std::cout << "Visited set: ";
						//for (auto a : visited)
					//	{
					//		std::cout << a << ",";
					//	}
					//	std::cout << '\n';
						//####END DEBUG STATEMENT#####
					}
				}
				queue.push(p);
			}
		}
	}
}
*/