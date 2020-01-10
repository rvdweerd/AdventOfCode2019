#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
#include "Day20HelperFunctions.h"

void Day20()
{
	MazeDay20::Maze maze;
	maze.PrintField();
	Coi2 startpos = maze.GetStartPos();
	Coi2 endpos = maze.GetEndPos();
	//auto vec = maze.GetNeighborsD20({ 2,8 });
	maze.ShortestPath(startpos, endpos);

}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day19b_demo();
	Day20();

	while (!_kbhit());
	return 0;
}


