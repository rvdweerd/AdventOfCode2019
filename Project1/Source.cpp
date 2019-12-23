#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
#include "Maze.h"

void MazeSolutionRec(Maze& maze, IntCode& mazeComputer, int lastDir)
{
	int computerOutput = 0;
	for (int dir = 4; dir > 0; dir--)
	{
		if (maze.Available(dir))
		{
			computerOutput = mazeComputer.Run15<int>(dir);
			if (computerOutput == 0)
			{
				maze.CreateWallNext(dir);
				//if ((maze.steps % 1000) == 0) maze.PrintMovingCenter();

				//maze.PrintSymbol();
			}
			else if (computerOutput == 1)
			{
				maze.CreateWallCurrent();
				maze.Move(dir);
				if ( (maze.steps % 100)==0 ) maze.PrintMovingCenter();
				maze.steps++;
				MazeSolutionRec(maze, mazeComputer,dir);
			}
			else if (computerOutput == -999)
			{
				std::cout << "error -999";
				std::cin.get();
			}
			else if (computerOutput == 2)
			{
				std::cout << "success!";
				return;// true;
			}
		}
		if (dir == 1)
		{
			maze.MoveBack(lastDir);
			//if ((maze.steps % 1) == 0) maze.PrintMovingCenter();
			if (maze.steps == 0)
			{
				int poi = 0;
			}

			maze.steps--;
			return;// false;
		}
	}
}

void Day15()
{
	IntCode mazeComputer("day15input.txt");
	Maze maze(20000,45000);
	//maze.PrintMap();
	MazeSolutionRec(maze, mazeComputer, 0);
	{
		//maze.PrintMap();
		std::cout << "success";
		std::cout << maze.steps;
	}
}

int MazeSolutionTest(Maze& maze, int lastDir)
{
	maze.stackDepth++;
	for (int dir = 4; dir > 0; dir--)
	{
		int computerOutput;
		if (maze.Available(dir))
		{
			computerOutput = maze.GetTestOutput(dir);
				//if (maze.GetMazeContentAt(dir) == 'O') computerOutput = 2;
				//else computerOutput = 1;
			if (computerOutput == 0)
			{
				maze.CreateWallNext(dir);
			}
			else if (computerOutput == 1)
			{
				maze.CreateWallCurrent();
				maze.Move(dir);
				//if ((maze.steps % 100) == 0) maze.PrintMovingCenter();
				maze.PrintMap(); maze.PrintSymbol();
				maze.steps++;
				if (MazeSolutionTest(maze, dir) == 2) return 2;
			}
			else if (computerOutput == -999)
			{
				std::cout << "error -999";
				std::cin.get();
			}
			else if (computerOutput == 2)
			{
				maze.Move(dir);
				maze.steps++;
				std::cout << "success!";
				maze.stackDepth--;
				return 2;// true;
			}
		}
		if (dir == 1)
		{
			maze.MoveBack(lastDir);
			//if ((maze.steps % 1) == 0) maze.PrintMovingCenter();
			maze.PrintMap(); maze.PrintSymbol();
			if (maze.steps == 0)
			{
				int poi = 0;
			}

			maze.steps--;
			maze.stackDepth--;
			return 0;// false;
		}
	}

}

void Day15test()
{
	Maze maze(25, 8);
	//std::ifstream in("day15test.txt");

	maze.PrintMap();
	maze.PrintSymbol();
	MazeSolutionTest(maze, 0);
	return;
}


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day15test();

	while (!_kbhit());
	return 0;
}

