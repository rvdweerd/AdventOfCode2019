#pragma once
#include "Planet.h"
#include <vector>
#include <string>
#include <Windows.h>
#include <iostream>
#include <cassert>

int OpposideDir(int dir)
{
	switch (dir)
	{
	case 1:
		return 2;
		break;
	case 2:
		return 1;
		break;
	case 3:
		return 4;
		break;
	case 4:
		return 3;
		break;
	}
	std::cout << "Error";
	assert( false );
	return -1;
}
Pos AddDirToPos(int dir, Pos pos)
{
	switch (dir)
	{
	case 1:
		return { pos.x , pos.y-1 };
		break;
	case 2:
		return { pos.x , pos.y+1 };
		break;
	case 3:
		return { pos.x-1 , pos.y };
		break;
	case 4:
		return { pos.x+1 , pos.y };
		break;
	}
	std::cout << "Error";
	assert(false);
	return { 0,0 };
}
class Maze
{
public:
	Maze(int width, int height, bool simulated, std::string filename)
		:
		fieldWidth(width),
		fieldHeight(height),
		nPanels(width* height),
		origin({ width / 2, height / 2 }),
		pos({ width / 2, height / 2 }),
		simulationMode(simulated)
	{
		LoadField();
		if (simulated) LoadTestField(filename);

		// setup winapi consolescreen
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) std::cout << "ERROR INITIALIZING WINAPI";

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) std::cout << "ERROR INITIALIZING WINAPI";
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}
	void LoadField()
	{
		field.resize(nPanels, ' ');
		field[pos.y * fieldWidth + pos.x] = ' ';
	}
	
public: // All methods related to test code (int code simulation)
	void LoadTestField(std::string filename)
	{
		// Note: size of maze in .txt file should match field size
		std::ifstream in(filename);
		//std::ifstream in("maze_layout.txt");
		//std::ifstream in("day15test.txt");
		while (!in.eof())
		{
			char ch;
			int i = 0;
			std::string str;
			for (ch = in.get(); !in.eof(); ch = in.get())
			{
				if (ch != '\n')
				{
					if (ch == 'x')
					{
						pos.y = i / fieldWidth;
						pos.x = i-pos.y*fieldWidth;
						ch = ' ';
					}
					testField.push_back(ch);
					i++;
				}
			}
		}
	}
	int GetTestOutput(int dir)
	{
		char targetfield=0;
		switch (dir)
		{
		case 1: //up
			targetfield = testField[(pos.y - 1) * fieldWidth + (pos.x)];
			break;
		case 2: //down
			targetfield = testField[(pos.y + 1) * fieldWidth + (pos.x)];
			break;
		case 3: //west
			targetfield = testField[(pos.y) * fieldWidth + (pos.x - 1)];
			break;
		case 4: //east
			targetfield = testField[(pos.y) * fieldWidth + (pos.x + 1)];
			break;
		}
		if (targetfield == 'O')  return 2;
		if (targetfield == '#')  return 0;
		if (targetfield == ' ')  return 1;
		if (targetfield == '.')  return 0;
		std::cout << "error";
		return -1;

	}
	bool AvailableInTestField(Pos p,int dir)
	{
		return (!BlockedInTestField(p,dir));
	}
	bool BlockedInTestField(Pos p, int dir)
	{
		switch (dir)
		{
		case 1: //up
			return testField[(p.y - 1) * fieldWidth + (p.x)] == '#' ||
				testField[(p.y - 1) * fieldWidth + (p.x)] == '.';
			break;
		case 2: //down
			return testField[(p.y + 1) * fieldWidth + (p.x)] == '#' ||
				testField[(p.y + 1) * fieldWidth + (p.x)] == '.';
			break;
		case 3: //west
			return testField[(p.y) * fieldWidth + (p.x - 1)] == '#' ||
				testField[(p.y) * fieldWidth + (p.x - 1)] == '.';
			break;
		case 4: //east
			return testField[(p.y) * fieldWidth + (p.x + 1)] == '#' ||
				testField[(p.y) * fieldWidth + (p.x + 1)] == '.';
			break;
		}
		assert(false);
		return false;
	}
public: // All methods used to navigate the map (using the int code responses)
	bool Available(int dir)
	{
		return (!Blocked(dir));
	}
	bool Blocked(int dir)
	{
		switch (dir)
		{
		case 1: //up
			return field[(pos.y - 1) * fieldWidth + (pos.x)] == '#' ||
				field[(pos.y - 1) * fieldWidth + (pos.x)] == '.';
			break;
		case 2: //down
			return field[(pos.y + 1) * fieldWidth + (pos.x)] == '#' ||
				field[(pos.y + 1) * fieldWidth + (pos.x)] == '.';
			break;
		case 3: //west
			return field[(pos.y) * fieldWidth + (pos.x - 1)] == '#' ||
				field[(pos.y) * fieldWidth + (pos.x - 1)] == '.';
			break;
		case 4: //east
			return field[(pos.y) * fieldWidth + (pos.x + 1)] == '#' ||
				field[(pos.y) * fieldWidth + (pos.x + 1)] == '.';
			break;
		}
		assert(false);
		return false;
	}
	void CreateWallNext(int dir)
	{
		switch (dir)
		{
		case 1: //up
			field[(pos.y - 1) * fieldWidth + (pos.x)] = '#';
			//PrintTextToConsole("#", { (SHORT)pos.x , (SHORT)pos.y - 1 });
			break;
		case 2: //down
			field[(pos.y + 1) * fieldWidth + (pos.x)] = '#';
			//PrintTextToConsole("#", { (SHORT)pos.x ,(SHORT)pos.y + 1 });
			break;
		case 3: //west
			field[(pos.y) * fieldWidth + (pos.x - 1)] = '#';
			//PrintTextToConsole("#", { (SHORT)pos.x-1 ,(SHORT)pos.y});
			break;
		case 4: //east
			field[(pos.y) * fieldWidth + (pos.x + 1)] = '#';
			//PrintTextToConsole("#", { (SHORT)pos.x+1 ,(SHORT)pos.y });
			break;
			assert(false);
		}
		return;
	}
	void LeaveTrailAtCurrentPos()
	{
		field[(pos.y) * fieldWidth + (pos.x)] = '.';
	}
	void MoveTo(int dir)
	{
		//PrintTextToConsole(".", { (SHORT)pos.x,(SHORT)pos.y });
		switch (dir)
		{
		case 1://up
			pos.y--;
			field[pos.y * fieldWidth + pos.x] = '.';
			return;
			break;
		case 2://down
			pos.y++;
			field[pos.y * fieldWidth + pos.x] = '.';
			return;
			break;
		case 3://west
			pos.x--;
			field[pos.y * fieldWidth + pos.x] = '.';
			return;
			break;
		case 4://east
			pos.x++;
			field[pos.y * fieldWidth + pos.x] = '.';
			return;
			break;
		}
		assert(false);
		//PrintTextToConsole("x", { (SHORT)pos.x,(SHORT)pos.y });
	}
	void MoveBack(int dir)
	{
		dir = OpposideDir(dir);
		//PrintTextToConsole(" ", { (SHORT)pos.x,(SHORT)pos.y });
		switch (dir)
		{
		case 1://up
			field[pos.y * fieldWidth + pos.x] = ' ';
			pos.y--;
			field[pos.y * fieldWidth + pos.x] = ' ';
			break;
		case 2://down
			field[pos.y * fieldWidth + pos.x] = ' ';
			pos.y++;
			field[pos.y * fieldWidth + pos.x] = ' ';

			break;
		case 3://west
			field[pos.y * fieldWidth + pos.x] = ' ';
			pos.x--;
			field[pos.y * fieldWidth + pos.x] = ' ';
			break;
		case 4://east
			field[pos.y * fieldWidth + pos.x] = ' ';
			pos.x++;
			field[pos.y * fieldWidth + pos.x] = ' ';
			break;
			assert(false);
		}
		return;
		//PrintTextToConsole("x", { (SHORT)pos.x,(SHORT)pos.y });
	}
	char GetMazeContentAt(int dir)

	{
		switch (dir)
		{
		case 1: //up
			return field[(pos.y - 1) * fieldWidth + (pos.x)];
			break;
		case 2: //down
			return field[(pos.y + 1) * fieldWidth + (pos.x)];
			break;
		case 3: //west
			return field[(pos.y) * fieldWidth + (pos.x - 1)];
			break;
		case 4: //east
			return field[(pos.y) * fieldWidth + (pos.x + 1)];
			break;
		}
		assert(false);
		return -1;
	}
	void ExploreCurrentPosition(IntCode c) // not by reference to avoid screwups
	{
		if (simulationMode)
		{
			for (int dir = 1; dir < 5; dir++)
			{
				int computerOutput = GetTestOutput(dir);
				if (computerOutput == 0)
				{
					CreateWallNext(dir);
				}
			}
		}
		else
		{
			for (int dir = 1; dir < 5; dir++)
			{
				int computerOutput = c.Run15<int>(dir);
				if (computerOutput == 0)
				{
					CreateWallNext(dir);
				}
				else if (computerOutput == 1)
				{
					c.Run15<int>(OpposideDir(dir));
				}
				else if (computerOutput == 2)
				{
					/* MANUAL MODE
					Pos pos_orig = pos;
					MoveTo(dir);
					PrintMaze();
					int num=0;
					while (num != 9)
					{
						PrintTextToConsole("input (9=quit): ", { 0,7 });
						std::cin >> num;
						{
							if (num > 0 && num < 5)
							{
								int cOutLocal = c.Run15<int>(num);
								if (cOutLocal == 0)
								{
									CreateWallNext(num);
								}
								else if (cOutLocal == 1 || cOutLocal == 2)
								{
									MoveTo(num);
								}
							}
						}
						PrintMaze();
					}
					pos = pos_orig;
					dir = 100;
					*/
					c.Run15<int>(OpposideDir(dir));
				}
			}
		}
	}
	void PrintMaze(bool fixed)
	{
		if (fixed)
		{
			PrintMap(); PrintTextToConsole("O", { (SHORT)pos.x,(SHORT)pos.y });
		}
		else PrintMovingCenter(min(40,fieldWidth/2),min(40,fieldHeight /2));
	}
	void PrintTestMaze()
	{
		PrintTestMap();// PrintTextToConsole("O", { (SHORT)pos.x,(SHORT)pos.y });
	}
	void PrintTextToConsole(std::string text, COORD coord)
	{
		SetConsoleCursorPosition(hStdOut, coord);
		std::cout << text;
	}
	bool IsInSimulationMode()
	{
		return simulationMode;
	}

private: // Print support functions
	void PrintMap()
	{
		ClearScreen();
		SetConsoleCursorPosition(hStdOut, homeCoords);
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				std::cout << field[y * fieldWidth + x];
			}
			std::cout << std::endl;
		}

	}
	void PrintTestMap()
	{
		ClearScreen();
		SetConsoleCursorPosition(hStdOut, homeCoords);
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				std::cout << testField[y * fieldWidth + x];
			}
			std::cout << std::endl;
		}
	}
	void PrintMovingCenter(int w, int h)
	{

		for (int y = 0; y < w; y++)
		{
			for (int x = 0; x < h; x++)
			{
				SetConsoleCursorPosition(hStdOut, { (SHORT)x, (SHORT)y });
				std::cout << field[(pos.y - w/2 + y) * fieldWidth + (pos.x - h/2 + x)];
			}
		}
		SetConsoleCursorPosition(hStdOut, { (SHORT)h/2, (SHORT)w/2 });
		std::cout << 'O';
	}
	void PrintSymbol()
	{
		PrintTextToConsole("O", { (SHORT)pos.x,(SHORT)pos.y });
	}

public: // quick fix; should be kept private
	std::vector<char> field;
	Pos pos;
	std::vector<char> testField;
	bool errorflag = false;
private:
	const int fieldWidth;
	const int fieldHeight;
	int nPanels;
	const std::pair<int, int> origin;
	const int trailLength = 5;
	bool simulationMode;
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
