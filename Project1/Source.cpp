#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
#include "Maze.h"
#include <cassert>
#include <random>
#include <fstream>
#include <iostream>

bool MazeSolutionExists(Maze& maze, IntCode& mazeComputer, int lastDir)
{
	//Recursive backtracking algo, used to pathtrace in maze for Day15 part 1
	maze.stackDepth++;
	for (int dir = 1; dir <5; dir++)
	{
		int computerOutput=-999;
		if (maze.totalsteps%20==0)
		{
			maze.PrintMaze();
		}
		if (maze.Available(dir))
		{
			if (maze.IsInSimulationMode()) computerOutput = maze.GetTestOutput(dir);
			else computerOutput = mazeComputer.Run15<int>(dir);

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
				maze.PrintMaze();
				maze.PrintTextToConsole("success! ", { 0,8 });
				std::cout << "End position: ("<< maze.pos.x<<","<<maze.pos.y<<"); nSteps: "<<maze.steps;
				maze.stackDepth--;
				return true;
			}
		}
		if (dir == 4)
		{
			maze.MoveBack(lastDir);
			mazeComputer.Run15<int>(OpposideDir(lastDir));
			maze.steps--;
			maze.stackDepth--;
			return false;
		}

	}
	maze.PrintTextToConsole("error stack empty", { 0,8 }); //std::cout << "Error";
	return false;
}
void Day15testRandom()
{
	// Approach: make random moves though the maze until you find the target
	// (very inefficient but it works. Number of moves will differ between runs
	
	// Initialization
	IntCode mazeComputer("day15input.txt");
	std::random_device rn;
	std::mt19937 rng(rn());
	std::uniform_int_distribution<int> distr(1, 4);
	std::vector<int> instructions;
	int response = 0;

	std::cout << "Hello"<<instructions.size() << std::endl;

	// Random search until you find target (computer response 2 means target is found)
	while (response != 2)
	{
		int move = distr(rng);

		response = mazeComputer.Run15(move);
		if (response == 1 || response == 2) instructions.push_back(move);
		if (instructions.size() % 100000 == 0)
		{
			std::cout << instructions.size() << std::endl;
		}
	}
	
	// Translate move commands to positions on the maze
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

	// i for loop on positions
	auto positions_copy = positions;
	int len = positions_copy.size();

	for (int i = 0; i < len; i++)
	{
		for (int j = i + 1; j < len; j++)
		{
			if (positions_copy[j] == positions_copy[i])
			{
				positions_copy.erase(positions_copy.begin() + i + 1, positions_copy.begin() + j+1);
				instructions.erase(instructions.begin() + i + 1, instructions.begin() + j + 1);
				len = positions_copy.size();
				i--;
				break;
			}
		}
	}
	
	// Remove all bactracking paths from positions (a vector of all (x,y) positions traversed)
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

	// Save instructions vector to file "maze_instr.bin"
	// first: write size of vector, then write all elements
	{
		std::ofstream out("maze_instr.bin", std::ios::binary);
		int vecsize = instructions.size();
		out.write(reinterpret_cast<char*>(&vecsize), sizeof vecsize);
		for (int v : instructions)
		{
			out.write(reinterpret_cast<char*>(&v), sizeof v);
		}
		out.close();
	}
	return;

}
void Day15applyPath()
{
	// Day15testRandom() should be run first to write data to maze_instr.bin
	IntCode mazeComputer("day15input.txt");
	Maze maze(20000, 45000, false);
	maze.PrintMaze();

	{
		std::vector<int> instructions;
		std::ifstream in("maze_instr.bin", std::ios::binary);
		int vecsize = 0;
		in.read(reinterpret_cast<char*>(&vecsize), sizeof vecsize);
		for (int i = 0; i < vecsize; i++)
		{
			int inval;
			in.read(reinterpret_cast<char*>(&inval), sizeof inval);
			instructions.push_back(inval);
		}
		int count = 0;
		for (int v : instructions)
		{
			//if (++count == 80)break;
			for (int move = 1; move < 5; move++)
			{
				int response = mazeComputer.Run15(move);
				if (response == 0)
				{
					maze.CreateWallNext(move);
				}
				else if (response == 1)
				{
					maze.MoveTo(move);
					maze.LeaveTrailAtCurrentPos();
					maze.MoveTo(OpposideDir(move));
					mazeComputer.Run15(OpposideDir(move));
				}
			}
			int computerOutput = mazeComputer.Run15<int>(v);
			if (computerOutput == 1) maze.MoveTo(v);
			//maze.PrintMaze();
		}maze.PrintMaze();
	}

}
void Day15simulated()
{
	// this runs on a small custom made maze to test the backtracking algo 
	// (no int computer involved to make debugging easier)
	IntCode mazeComputer("day15input.txt");
	//Maze maze(20000, 45000);
	//maze.PrintMovingCenter();
	
	Maze maze(25, 8, true);
	maze.PrintMaze();
	MazeSolutionExists(maze, mazeComputer, 4);
	return;
}
void Day15a()
{
	IntCode mazeComputer("day15input.txt");
	Maze maze(72, 72, false);
	MazeSolutionExists(maze, mazeComputer, 4);

	// Write field vector of maze to "maze_layout.bin", binary mode
	/*
	{
		std::ofstream out("maze_layout.bin", std::ios::binary);
		int vecsize = maze.field.size();
		out.write(reinterpret_cast<char*>(&vecsize), sizeof vecsize);
		for (char c : maze.field)
		{
			out.write(reinterpret_cast<char*>(&c), sizeof c);
		}
		out.close();
	}
	*/
	// Write field vector of maze to "maze_layout.txt", text mode
	/*
	{
		std::ofstream out("maze_layout.txt", std::ios::out);
		int count = 0;
		for (char c : maze.field)
		{
			out<<c;
			count++;
			if (count % 72 == 0) out << '\n';
		}
		out.close();
	}
	*/
	return;
}
void Day15b()
{

}
int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day15testRandom();
	Day15a();
	//Day15applyPath();

	while (!_kbhit());
	return 0;
}

