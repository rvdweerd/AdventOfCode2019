#pragma once
#include "IncludesUsed.h"
static bool route_compare(std::pair<std::string, int> a, std::pair<std::string, int> b)
{
	return (a.second < b.second);
}
struct Coi2
{
	int x;
	int y;
};
struct Key
{
	char keychar;
	Coi2 coordinates;
};
struct KeyPosition
{
	Key key;
	std::string keys;
	int steps = 0;
	std::string path;
};
struct CacheInfo
{
	std::vector<std::pair<Key, int>> cachedAvailablePositions;
	int cumulativeSteps;
};
struct DiagnosticData
{
	int loopCount_OuterBFS=0;
	int loopCount_InnerBFS=0;
	int MaxQueueSize_OuterBFS=0;
	int MaxQueueSize_InnerBFS=0;
};
struct Vei2
{
	std::vector<int> coord = { 0,0 };
	std::string keys;
	int steps = 0;
};
bool IsKey(char c)
{
	return ((c > 96 && c <= 122) || (c == '@'));
}
bool IsDoor(char c)
{
	return (c > 64 && c <= 90);
}
std::string Hash(const KeyPosition& p) // used to enable a hash lookup of visited nodes in the BFS
{
	std::string keys = p.keys;
	std::sort(keys.begin(), keys.end());
	std::string Hash = "";
	Hash +=p.key.keychar;
	Hash.append(keys);
	return Hash;
}
std::string Hash(const Vei2& p0)
{
	return std::to_string(p0.coord[0]) + "_" + std::to_string(p0.coord[1]);
}
Coi2 LoadField(const std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys, std::map<char, int>& keyIndices, std::map<char,Coi2>& keyPositions)
{
	fieldWidth = 1; // we don't know this yet, this is the starting assumption
	nKeys = 0;
	bool fieldWidthSet = false;
	std::ifstream in(filename);
	Coi2 pos0;
	while (!in.eof())
	{
		char ch;
		std::string str;
		int i = 0;
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
				if (IsKey(ch))
				{
					keyIndices[ch] = i;
					int keyY = i / fieldWidth;
					int keyX = i - keyY * fieldWidth;
					keyPositions[ch] = { keyX , keyY };
					nKeys++;
				}
				if (ch == '@')
				{
					{
						pos0.y = i / fieldWidth;
						pos0.x = i - pos0.y * fieldWidth;
					}
				}
				field.push_back(ch);
				i++;
			}
		}
	}
	return pos0;
}
void PrintField(std::vector<char> field, const int& fieldWidth)
{
	//std::cout << '\n';
	for (int i = 0; i < (int)field.size(); i++)
	{
		if (i % fieldWidth == 0) std::cout << '\n';
		/*if ((i > 2 * fieldWidth && (i < (int)field.size() - 2 * fieldWidth)) && //away from top/bottom
			(field[i - fieldWidth] == '.' || field[i - fieldWidth] > 65) &&
			(field[i + fieldWidth] == '.' || field[i + fieldWidth] > 65) && // above and below is corridor
			(field[i] == '#')) // cell is barrier
		{
			std::cout << '-';
		}
		//else if (field[max(0,i - 1)] == '.' && field[i + 1] == '.' && field[i] == '#')
	//	{
	//		std::cout << '|';
	//	}
		else if (field[i] == '#')
		{
			std::cout << '+';
		}
		else if (field[i] == '.')
		{
			std::cout << ' ';
		}
		else*/
		{
			std::cout << field[i];
		}
	}
}
std::vector<Vei2> GetNewCoordinates(Vei2& curPos, const std::vector<char>& field, const int& fieldWidth, std::set<std::string>& visited)
{
	std::vector<Vei2> NewCoordinates;
	for (unsigned dir = 4; dir >0; dir--)
	{
		Vei2 newPos = curPos;
		if (dir == 1) newPos.coord[1]--;		// NORTH
		else if (dir == 2) newPos.coord[1]++;	// SOUTH
		else if (dir == 3) newPos.coord[0]--;	// WEST
		else if (dir == 4) newPos.coord[0]++;	// EAST

		char ch = field[newPos.coord[1] * fieldWidth + newPos.coord[0]];
		if (ch != '#')
		{
			if (visited.find(Hash(newPos)) == visited.end()) // New position was not previsously visited in current state
			{
				NewCoordinates.push_back(newPos);
			}
		}
	}
	return NewCoordinates;
}

std::vector<std::pair<Key,int>> GetAvailableKeyPositions(
	KeyPosition& curKeyPos, 
	const std::vector<char>& field, 
	const int& fieldWidth, 
	std::set<std::string>& visitedKeys,
	std::map<std::string, CacheInfo>& cache,
	DiagnosticData& diagData )
{
	std::vector<std::pair<Key, int>> availableKeyOptions;

	auto cache_it = cache.find(Hash(curKeyPos));
	if (cache_it == cache.end())
	{

		Vei2 p0 = { {curKeyPos.key.coordinates.x,curKeyPos.key.coordinates.y},curKeyPos.keys,curKeyPos.steps };
		std::set<std::string> visited;
		visited.insert(Hash(p0));
		std::queue<Vei2> queue;
		queue.push(p0);

		while (!queue.empty())
		{
			if (queue.size() > diagData.MaxQueueSize_InnerBFS) diagData.MaxQueueSize_InnerBFS = queue.size();
			diagData.loopCount_InnerBFS++;
			Vei2 curPos = queue.front(); queue.pop();
			std::vector<Vei2> newPositions = GetNewCoordinates(curPos, field, fieldWidth, visited);
			for (Vei2 newPos : newPositions)
			{
				// New status allocation
				newPos.steps++;
				char ch = field[newPos.coord[1] * fieldWidth + newPos.coord[0]];

				if (IsKey(ch) && (newPos.keys.find(ch) == std::string::npos)) // If newpos is a new key => add it to newKeyPositions, don't push to queue
				{
					Key newkey = { ch,{newPos.coord[0],newPos.coord[1]} };
					availableKeyOptions.push_back({ newkey,newPos.steps - curKeyPos.steps });
				}
				else if (IsDoor(ch) && (newPos.keys.find(std::tolower(ch)) == std::string::npos)) // If Door is found, you don't have the key => don't push to queue
				{
				}
				else // continue with new position
				{
					visited.insert(Hash(newPos));
					queue.push(newPos);
				}
			}
		}
		// Update cache
		cache[Hash(curKeyPos)] = { availableKeyOptions, curKeyPos.steps };
	}
	else
	{
		if (cache_it->second.cumulativeSteps <= curKeyPos.steps)
		{
			return {};
		}
		else
		{
			cache_it->second.cumulativeSteps = curKeyPos.steps;
			availableKeyOptions = cache_it->second.cachedAvailablePositions;
			//std::cout << "cache used.";
		}
	}
	return availableKeyOptions;
}