#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
#include "Day20HelperFunctions.h"

void Day20a()
{
	bool recursiveMazeOption = false;
	MazeDay20::Maze maze(recursiveMazeOption);
	Coi2 startpos = maze.GetStartPos();
	Coi2 endpos = maze.GetEndPos();
	int answer = maze.ShortestPath(startpos, endpos);
	std::cout << "Steps to exit: " << answer;
	std::cout << "\nPress Enter to continue."; std::cin.get();
}
void Day20b()
{
	bool recursiveMazeOption = true;
	MazeDay20::Maze maze(recursiveMazeOption);
	Coi2 startpos = maze.GetStartPos();
	Coi2 endpos = maze.GetEndPos();
	int answer = maze.ShortestPath(startpos, endpos);
	std::cout << "Steps to exit: " << answer;
}


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day20a();
	Day20b();

	while (!_kbhit());
	return 0;
}


