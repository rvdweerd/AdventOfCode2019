#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
#include "Maze.h"
#include <cassert>
#include <random>
//#include <utility>

bool MazeSolutionExists(Maze& maze, IntCode& mazeComputer, int lastDir)
{
	maze.stackDepth++;
	for (int dir = 4; dir > 0; dir--)
	{
		//if (maze.totalsteps % 1 == 0) maze.PrintMovingCenter();
		//maze.PrintMap(); maze.PrintTextToConsole("x", { (SHORT)maze.pos.x,(SHORT)maze.pos.y });
		int computerOutput;
		if (maze.Available(dir))
		{
			computerOutput = maze.GetTestOutput(dir);
			//computerOutput = mazeComputer.Run15<int>(dir);
			if (computerOutput == 0)
			{
				maze.CreateWallNext(dir);
			}
			else if (computerOutput == 1)
			{
				maze.LeaveTrailAtCurrentPos();
				maze.MoveTo(dir);
				maze.steps++;maze.totalsteps++;
				if (MazeSolutionExists(maze, mazeComputer, dir)) return true;
			}
			else if (computerOutput == -999)
			{
				std::cout << "error -999";
				std::cin.get();
			}
			else if (computerOutput == 2)
			{
				maze.MoveTo(dir);
				maze.steps++;maze.totalsteps++;
				std::cout << "success!" << maze.pos.x<<","<<maze.pos.y;
				maze.stackDepth--;
				return true;
			}
		}
		if (dir == 1)
		{
			maze.MoveBack(lastDir);
			maze.steps--;
			if (maze.steps == 10) 
			{
				//maze.PrintMovingCenter();
				maze.errorflag = true;
			}
			//if (maze.errorflag) maze.PrintMovingCenter();
			maze.stackDepth--;
			return false;
		}
	}
	maze.PrintTextToConsole("error stack empty", { 0,30 }); //std::cout << "Error";
	return false;
}

void Day15testRandom()
{
	IntCode mazeComputer("day15input.txt");
	//Maze maze(20000, 45000);
	//maze.PrintMovingCenter();

	//Maze maze(25, 8);
	//maze.PrintMap();
	//maze.PrintSymbol();
	//MazeSolutionExists(maze, mazeComputer, 4);
	std::random_device rn;
	std::mt19937 rng(rn());
	std::uniform_int_distribution<int> distr(1, 4);
	std::vector<int> instructions;
	int response = 0;
	while (response != 2)
	{
		int move = distr(rng);
		response = mazeComputer.Run15(move);
		if (response == 1) instructions.push_back(move);
	}

	std::pair<int, int> pos = { 0,0 };
	std::vector<std::pair<int, int>> positions;
	for (int in : instructions)
	{
		if (in == 1) pos.second--;
		if (in == 2) pos.second++;
		if (in == 3) pos.first--;
		if (in == 4) pos.first++;
		positions.push_back(pos);
	}
	for (auto i = positions.begin(), e = positions.end(); i != e; )
	{
		auto f = std::find(i + 1, e, *i);
		if (f != e)
		{
			i = positions.erase(i + 1, f + 1);
			e = positions.end();
			//e = vec.end();
			i--;
		}
		else
		{
			i++;
		}
	}

	std::cout << "Success, n=" << positions.size();
	return;

}
void Day15test()
{
	IntCode mazeComputer("day15input.txt");
	//Maze maze(20000, 45000);
	//maze.PrintMovingCenter();
	
	Maze maze(25, 8);
	maze.PrintMap();
	maze.PrintSymbol();
	MazeSolutionExists(maze, mazeComputer, 4);
	
	return;
}


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day15testRandom();

	while (!_kbhit());
	return 0;
}

