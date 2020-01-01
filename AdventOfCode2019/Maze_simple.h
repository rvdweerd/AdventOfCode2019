/* ABANDONED CODE
#pragma once
#include "Planet.h"
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <cassert>
#include "HelperFunctions.h"
#include <queue>

struct pathToKey
{
	Pos endpos;
	int distance;
	char endKeyChar;
};
class Maze_simple
{
public:
	Maze_simple(std::string filename)
	{
		LoadField(filename);

		// setup winapi consolescreen
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) std::cout << "ERROR INITIALIZING WINAPI";

		// Get the number of cells in the current buffer //
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) std::cout << "ERROR INITIALIZING WINAPI";
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		// Fill the entire buffer with spaces //
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		// Fill the entire buffer with the current colors and attributes //
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		// Move the cursor home //
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}
	void LoadField(std::string filename)
	{
		fieldWidth = 1; // we don't know this yet, this is the starting assumption
		bool fieldWidthSet = false;
		std::ifstream in(filename);
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
						pos.y = i / fieldWidth;
						pos.x = i - pos.y * fieldWidth;
						startpos = pos;
					}
					field.push_back(ch);
					i++;
				}
			}
		}
		nPanels = field.size();
		fieldHeight = nPanels / fieldWidth;

		// Scan for end key in the maze
		for (char c : field)
		{
			if (c > endkey)
			{
				endkey = c;
			}
			if (c >= 97 && c < 123)
			{
				nKeys++;
			}
		}
	}

public: // Old methods used to navigate the map 
	bool Available(int dir, const std::vector<char>& keys)
	{
		return (!Blocked(dir,keys));
	}
	bool Blocked(int dir, const std::vector<char>& keys)
	{
		int fieldindex=-1;
		switch (dir)
		{
		case 1: //up
			fieldindex = (pos.y - 1) * fieldWidth + (pos.x);
			break;
		case 2: //down
			fieldindex = (pos.y + 1) * fieldWidth + (pos.x);
			break;
		case 3: //west
			fieldindex = (pos.y ) * fieldWidth + (pos.x-1); 
			break;
		case 4: //east
			fieldindex = (pos.y) * fieldWidth + (pos.x+1);
			break;
		}
		if (fieldindex == -1) return true;
		char c = field[fieldindex];
		if (c == '#') return true;
		else if (c >= 65 && c < 91) // captials letter encountered
		{
			if (std::find(keys.begin(), keys.end(), std::tolower(c)) == keys.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else return false;
	}
	void MoveTo(int dir, std::vector<char>& keys)
	{
		//PrintTextToConsole(".", { (SHORT)pos.x,(SHORT)pos.y });
		field[size_t(pos.y * fieldWidth + pos.x)] = '.';
		switch (dir)
		{
		case 1://up
			pos.y--;
			break;
		case 2://down
			pos.y++;
			break;
		case 3://west
			pos.x--;
			break;
		case 4://east
			pos.x++;
			break;
		}
		char c = field[size_t(pos.y * fieldWidth + pos.x)];
		field[size_t(pos.y * fieldWidth + pos.x)] = '@';
		if (c >= 97 && c < 123) // lowercase letter encountered
		{
			keys.push_back(std::tolower(c));
		}
		return;
	}
public: // methods for solution Day18
	char GetMazeContentFromPosInDir(Pos p, int dir)
	{
		switch (dir)
		{
		case 1: //up
			return field[(p.y - 1) * fieldWidth + (p.x)];
			break;
		case 2: //down
			return field[(p.y + 1) * fieldWidth + (p.x)];
			break;
		case 3: //west
			return field[(p.y) * fieldWidth + (p.x - 1)];
			break;
		case 4: //east
			return field[(p.y) * fieldWidth + (p.x + 1)];
			break;
		}
		assert(false);
		return -1;
	}
	char GetMazeContentAtPos(Pos p)
	{
		return field[(p.y ) * fieldWidth + (p.x)];
	}
	Pos GetStartPos()
	{
		return startpos;
	}
	std::vector<int> GetAvailableDirectionsFromPos(Pos p, const std::string keys)
	{
		std::vector<int> availableDirs;
		for (int dir = 1; dir < 5; dir++)
		{
			char ch = GetMazeContentFromPosInDir(p,dir);
			// direction is not available in two cases:
			if ((ch == '#') || // there is a wall;
				((ch >= 65 && ch < 91) && std::find(keys.begin(), keys.end(), std::tolower(ch)) == keys.end()))  // capital letter encountered and you don't have the key )
			{
			}
			else
			{
				availableDirs.push_back(dir);
			}
		}
		return availableDirs;
	}
	void BFSforKeys(std::vector<pathToKey>& pathToKeys, 
					//std::queue<Pos>& newPositionsQueue, 
					//std::vector<Pos> visited, //By reference?
					Pos startpos,
					std::string keys, 
					int nSteps,
					std::map<std::string, std::vector<pathToKey>>& cache)
	{
		std::vector<Pos> allCheckedPositions;
		allCheckedPositions.push_back(startpos);

		// get all available directions from startposition and initialize the queue for BFS
		std::queue<Pos> newPositionsQueue;
		//std::string edgeStart = keys;
		std::vector<Pos> visited; visited.push_back(startpos);
		for (int dir : GetAvailableDirectionsFromPos(startpos, keys))
		{
			Pos temppos = startpos.Add(dir);
			temppos.n++;
			newPositionsQueue.push(temppos);
		}
		// main BFS loop
		while (newPositionsQueue.size() != 0)
		{
			Pos newpos = newPositionsQueue.front(); newPositionsQueue.pop();
			allCheckedPositions.push_back(newpos);
			visited.push_back(newpos);
			char ch = GetMazeContentAtPos(newpos);
			// If you find a new key, add it to the vector
			if ((ch >= 97 && ch < 123) && std::find(keys.begin(), keys.end(), ch) == keys.end())
			{
				pathToKey path = { newpos,newpos.n,ch };
				pathToKeys.push_back(path);
			}
			// Else, keep looking
			else
			{
				
				for (int dir : GetAvailableDirectionsFromPos(newpos, keys))
				{
					Pos nextpos = newpos.Add(dir);
					nextpos.n++;
					if (std::find(visited.begin(),visited.end(),nextpos) == visited.end())
					{	
						newPositionsQueue.push(nextpos);
					}
				}

			}
		}
		// Write result to the cache
		for (Pos p : allCheckedPositions)
		{
			std::string orderedkeys = keys;
			std::sort(orderedkeys.begin(), orderedkeys.end());
			orderedkeys += std::to_string(p.x);
			orderedkeys += "_";
			orderedkeys += std::to_string(p.y);
			cache[orderedkeys] = pathToKeys;
		}
		return;
	}
	void PrintMaze()
	{
		PrintMap(); 
	}
	void PrintTextToConsole(std::string text, COORD coord)
	{
		SetConsoleCursorPosition(hStdOut, coord);
		std::cout << text;
	}
private: // Print support functions
	void PrintMap()
	{
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				std::cout << field[size_t(y * fieldWidth + x)];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	void PrintSymbol()
	{
		PrintTextToConsole("@", { (SHORT)pos.x,(SHORT)pos.y });
	}

public: // quick fix; should be kept private
	std::vector<char> field;
	Pos pos;
	Pos startpos;
	bool errorflag = false;
	char endkey='a';
	int nKeys;
public:
	int fieldWidth;
	int fieldHeight;
	int nPanels;
	//const std::pair<int, int> origin;
public:
	int steps = 0;
	int totalsteps = 0;
	int stackDepth = 0;
	int totalbacksteps = 0;
private:
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };
};
*/