#pragma once
#include "IncludesUsed.h"

struct Vei2
{
	std::vector<int> coord = { 0,0 };
	std::string keys;
	int steps = 0;
};
struct Vei8
{
	std::vector<int> coord = { 0,0,0,0,0,0,0,0 };
	std::string keys;
	int steps = 0;
};
std::string Hash(const Vei2& pos) // used to enable a hash lookup of visited nodes in the BFS
{
	std::string keys = pos.keys;
	std::sort(keys.begin(), keys.end());
	std::string Hash = "(";
	Hash.append(std::to_string(pos.coord[0]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[1]));
	Hash.append(")[");
	Hash.append(keys);
	Hash.append("]");
	return Hash;
}
std::string Hash4C(const Vei8& pos,std::string keys) // used to enable a hash lookup of visited nodes in the BFS
{
	std::sort(keys.begin(), keys.end());
	std::string Hash = "(";
	Hash.append(std::to_string(pos.coord[0]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[1]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[2]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[3]));	
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[4]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[5]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[6]));
	Hash.append(",");
	Hash.append(std::to_string(pos.coord[7]));
	Hash.append(")[");
	Hash.append(keys);
	Hash.append("]");
	return Hash;
}

Vei2 LoadField(const std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys)
	{
		fieldWidth = 1; // we don't know this yet, this is the starting assumption
		bool fieldWidthSet = false;
		std::ifstream in(filename);
		Vei2 Pos0;
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
					if (ch == '@')
					{
						Pos0.coord[1] = i / fieldWidth;
						Pos0.coord[0] = i - Pos0.coord[1] * fieldWidth;
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
		return Pos0;
	}
Vei8 LoadField4C(const std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys)
{
	fieldWidth = 1; // we don't know this yet, this is the starting assumption
	bool fieldWidthSet = false;
	std::ifstream in(filename);
	Vei8 Pos0;
	while (!in.eof())
	{
		char ch;
		std::string str;
		int i = 0;
		int dim = 0;
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
					Pos0.coord[dim+1] = i / fieldWidth;
					Pos0.coord[dim] = i - Pos0.coord[dim+1] * fieldWidth;
					dim += 2;
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
	return Pos0;
}
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
		else
		{
			std::cout << field[i];
		}
	}
	std::cout << "\n";
}
std::vector<Vei2> GetNewCoordinates(Vei2& curPos, const std::vector<char>& field, const int& fieldWidth, std::set<std::string>& visited)
{
	std::vector<Vei2> NewCoordinates;
	for (unsigned dir = 1; dir < 5; dir++)
	{
		Vei2 newPos = curPos;
		if (dir == 1) newPos.coord[1]--;		// NORTH
		else if (dir == 2) newPos.coord[1]++;	// SOUTH
		else if (dir == 3) newPos.coord[0]--;	// WEST
		else if (dir == 4) newPos.coord[0]++;	// EAST

		if (visited.find(Hash(newPos)) == visited.end()) // New position was not previsously visited in current state
		{
			char ch = field[newPos.coord[1] * fieldWidth + newPos.coord[0]];
			if ((ch > 64 && ch <= 90) && (curPos.keys.find(std::tolower(ch)) == std::string::npos) || // Door, no key
				(ch == '#'))  // Wall
			{
			} // Position is not available
			else
			{
				NewCoordinates.push_back(newPos);
			}
		}
	}
	return NewCoordinates;
}
std::vector<Vei8> GetNewCoordinates4C(Vei8& curPos, const std::vector<char>& field, const int& fieldWidth, std::set<std::string>& visited, const std::string& globalKeys, int mutex)
{
	std::vector<Vei8> NewCoordinates;

	//for (size_t i = 0; i < 4; i++) // loop the 4 coordinates
	size_t i = (size_t)mutex;
	{
		for (unsigned dir = 1; dir < 5; dir++)
		{
			Vei8 newPos = curPos;
			if		(dir == 1) newPos.coord[2*i+1]--;	// NORTH
			else if (dir == 2) newPos.coord[2*i+1]++;	// SOUTH
			else if (dir == 3) newPos.coord[2*i]--;		// WEST
			else if (dir == 4) newPos.coord[2*i]++;		// EAST
	//???????? // Moet dit met globalkeys of met localkeys (2x) ?????
			if (visited.find(Hash4C(newPos,globalKeys)) == visited.end()) // New position was not previsously visited in current state
			{
				char ch = field[newPos.coord[2*i+1] * fieldWidth + newPos.coord[2*i]];
				if ((ch > 64 && ch <= 90) && (globalKeys.find(std::tolower(ch)) == std::string::npos) || // Door, no key
					(ch == '#'))  // Wall
				{
				} // Position is not available
				else
				{
					NewCoordinates.push_back(newPos);
				}
			}
		}
	}
	return NewCoordinates;
}
