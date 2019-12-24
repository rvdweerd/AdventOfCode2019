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
class Maze
{
public:
	Maze(int width, int height)
		:
		fieldWidth(width),
		fieldHeight(height),
		nPanels(width* height),
		origin({ width / 2, height / 2 }),
		pos({ width / 2, height / 2 })
	{
		LoadField();
		LoadTestField();

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
	void LoadTestField()
	{
		std::ifstream in("day15test.txt");
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
	bool Available(int dir)
	{
		switch (dir)
		{
		case 1: //up
			return field[(pos.y - 1) * fieldWidth + (pos.x)] == ' ' ||
				field[(pos.y - 1) * fieldWidth + (pos.x)] == 'O';
			break;
		case 2: //down
			return field[(pos.y + 1) * fieldWidth + (pos.x)] == ' ' ||
				field[(pos.y + 1) * fieldWidth + (pos.x)] == 'O';
			break;
		case 3: //west
			return field[(pos.y) * fieldWidth + (pos.x - 1)] == ' '||
				field[(pos.y) * fieldWidth + (pos.x - 1)] == 'O';
			break;
		case 4: //east
			return field[(pos.y) * fieldWidth + (pos.x + 1)] == ' ' ||
				field[(pos.y) * fieldWidth + (pos.x + 1)] == 'O';
			break;
			assert(false);
			return false;
		}
		std::cout << "Error";
		assert(false);
		return false;
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
			assert(false);
			return false;
		}
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

	}
	void LeaveTrailAtCurrentPos()
	{
		field[(pos.y) * fieldWidth + (pos.x)] = '.';
		//PrintTextToConsole("=", { (SHORT)pos.x  ,(SHORT)pos.y });
	}
	void MoveTo(int dir)
	{
		//PrintTextToConsole(".", { (SHORT)pos.x,(SHORT)pos.y });
		switch (dir)
		{
		case 1://up
			pos.y--;
			field[pos.y * fieldWidth + pos.x] = '.';
			break;
		case 2://down
			pos.y++;
			field[pos.y * fieldWidth + pos.x] = '.';
			break;
		case 3://west
			pos.x--;
			field[pos.y * fieldWidth + pos.x] = '.';
			break;
		case 4://east
			pos.x++;
			field[pos.y * fieldWidth + pos.x] = '.';
			break;
			assert(false);
		}
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
		//PrintTextToConsole("x", { (SHORT)pos.x,(SHORT)pos.y });
	}
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
	void PrintMovingCenter()
	{
		
		for (int y = 0; y < 40; y++)
		{
			for (int x = 0; x < 80; x++)
			{
				SetConsoleCursorPosition(hStdOut, { (SHORT)x, (SHORT)y });
				std::cout << field[(pos.y-20+y)*fieldWidth+(pos.x-40+x)];
			}
		}
		SetConsoleCursorPosition(hStdOut, { (SHORT)40, (SHORT)20 });
		std::cout << 'O';
	}
	void PrintSymbol()
	{
		PrintTextToConsole("x", { (SHORT)pos.x,(SHORT)pos.y });
	}
	void PrintTextToConsole(std::string text, COORD coord)
	{
		SetConsoleCursorPosition(hStdOut, coord);
		std::cout << text;
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
public:
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
public:
	int steps = 0;
	int totalsteps = 0;
	int stackDepth = 0;
private:
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };
};
