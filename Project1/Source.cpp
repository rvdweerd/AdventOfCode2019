#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <conio.h>
#include "IntCode.h"
#include "HelperFunctions.h"
#include "Planet.h"
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <thread>
#include "PlanetSys.h"
#include <utility>


void Day1()
{
	std::ifstream in("day1input.txt");
	std::vector<int> mass;
	while (!in.eof())
	{
		std::string str;
		for (char ch = in.get(); ch != '\n' && !in.eof(); ch = in.get())
		{
			str += ch;
		}
		mass.push_back(std::stoi(str));
	}

	int fuel = std::accumulate(mass.begin(), mass.end(), 0, [](int cum, int m)
		{
			int count = 0;
			int additional = m;
			while (additional != 0)
			{
				additional = max(0, additional / 3 - 2);
				count += additional;
			}

			return cum + count;
		});

	std::cout << "Fuel needed = " << fuel;
}
void Day2a()
{
	IntCode comp("day2input.txt");
	comp.GetRunCodeVectorReference()[1] = 12;
	comp.GetRunCodeVectorReference()[2] = 2;
	comp.Run();
	std::cout << "Answer to part 1: "<<comp.GetRunCodeVectorReference()[0];
}
void Day2b()
{
	IntCode comp("day2input.txt");

	int noun = 0;
	int verb = 0;
	for (noun = 0; noun < 100; ++noun)
	{
		for (verb = 0; verb < 100; ++verb)
		{
			comp.Reset();
			comp.GetRunCodeVectorReference()[1] = noun; 
			comp.GetRunCodeVectorReference()[2] = verb;
			comp.Run();
			std::cout << noun << "," << verb << ": " << comp.GetRunCodeVectorReference()[0] << std::endl;
			if (comp.GetRunCodeVectorReference()[0] == 19690720) break;
		}
		if (comp.GetRunCodeVectorReference()[0] == 19690720) break;
	}
	std::cout << "\nRuncode vector at termintion: ";
	for (long long int v : comp.GetRunCodeVectorReference())
	{
		std::cout << v << ",";
	}
	std::cout << "\nAnswer to part 2: " << (100 * noun + verb);
}
void Day3()
{
	std::vector<std::vector<char>> map1;
	auto maxx = map1.max_size();
	std::vector<std::vector<char>> map2;
	std::vector<std::vector<char>> mergedMap;
	int fieldSize = 50;//25000;
	static constexpr int maxPrintableFieldSize = 60;
	std::vector<char> row(fieldSize);
	for (int i = 0; i < fieldSize; i++) row[i] = '.';
	for (int i = 0; i < fieldSize; i++) map1.push_back(row);
	map2 = map1;

	std::ifstream in("day3input.txt");
	//BUILD MAP1
	int x = fieldSize / 2;
	int y = fieldSize / 2;
	char ch = 0; char dir = 0; char dir_old = 0;
	while (!in.eof() && ch != '\n')
	{

		std::string str;
		for (ch = in.get(); ch != ',' && !in.eof() && ch != '\n'; ch = in.get())
		{
			str += ch;
		}
		dir = str[0];
		if (dir == 'L')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map1[y][x] = '+';
					dir_old = dir;
				}
				map1[y][--x] = '-';
			}
		}
		if (dir == 'R')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map1[y][x] = '+';
					dir_old = dir;
				}
				map1[y][++x] = '-';
			}
		}
		if (dir == 'U') // mirrored, down in the map space
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map1[y][x] = '+';
					dir_old = dir;
				}
				map1[++y][x] = '|';
			}
		}
		if (dir == 'D')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map1[y][x] = '+';
					dir_old = dir;
				}
				map1[--y][x] = '|';
			}
		}
	} map1[fieldSize / 2][fieldSize / 2] = 'o';

	//BUILD MAP2
	x = fieldSize / 2;
	y = fieldSize / 2;
	ch = 0; dir = 0; dir_old = 0;
	while (!in.eof() && ch != '\n')
	{

		std::string str;
		for (ch = in.get(); ch != ',' && !in.eof() && ch != '\n'; ch = in.get())
		{
			str += ch;
		}
		dir = str[0];
		if (dir == 'L')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map2[y][x] = '+';
					dir_old = dir;
				}
				map2[y][--x] = '-';
			}
		}
		if (dir == 'R')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map2[y][x] = '+';
					dir_old = dir;
				}
				map2[y][++x] = '-';
			}
		}
		if (dir == 'U') // mirrored, down in the map space
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map2[y][x] = '+';
					dir_old = dir;
				}
				map2[++y][x] = '|';
			}
		}
		if (dir == 'D')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				if (dir != dir_old)
				{
					map2[y][x] = '+';
					dir_old = dir;
				}
				map2[--y][x] = '|';
			}
		}
	}  map2[fieldSize / 2][fieldSize / 2] = 'o';

	//MERGE MAPS
	mergedMap = map1;
	int origin_x = fieldSize / 2;
	int origin_y = fieldSize / 2;
	std::vector<int> hits;
	for (size_t y = 0; y < map1.size(); y++)
	{
		for (size_t x = 0; x < map1[0].size(); x++)
		{
			if (map1[y][x] != '.' && map2[y][x] != '.' && map1[y][x] != 'o' && map2[y][x] != 'o')
			{
				mergedMap[y][x] = 'X';
				int manhattanDist = abs(int(x) - origin_x) + abs(int(y) - origin_y);
				if (manhattanDist != 0) hits.push_back(manhattanDist);
			}
			else if (map1[y][x] == '.' && map2[y][x] != '.')
			{
				mergedMap[y][x] = map2[y][x];
			}
		}
	}
	if (fieldSize <= maxPrintableFieldSize)
	{
		printMap(map1);
		printMap(map2);
		printMap(mergedMap);
	}

	std::cout << "Vector of hit distances: ";
	for (int v : hits) { std::cout << v << ","; }
	if (hits.size() > 0)
	{
		int minDist = *std::min_element(hits.begin(), hits.end());
		std::cout << "Manhattan distance to the closest intersection: " << minDist << std::endl;
	}
	else
	{
		std::cout << "No hits.\n";
	}

	// Put Hits in map1 and map2
	char nHits = 1;
	for (size_t y = 0; y < map1.size(); y++)
	{
		for (size_t x = 0; x < map1[0].size(); x++)
		{
			map1[y][x] = 'z';
			map2[y][x] = 'z';
			if (mergedMap[y][x] == 'X')
			{
				map1[y][x] = nHits++;
				map2[y][x] = map1[y][x];
			}
		}
	}

	if (fieldSize <= maxPrintableFieldSize)
	{
		printMap(map1);
		printMap(map2);
		printMap(mergedMap);
	}

	//Traverse map1.
	std::ifstream in2("day3input.txt");
	std::map<int, std::pair<int, int>> distdata;
	x = fieldSize / 2;
	y = fieldSize / 2;
	ch = 0; dir = 0; dir_old = 0;
	int counter = 0;
	while (!in2.eof() && ch != '\n')
	{

		std::string str;
		for (ch = in2.get(); ch != ',' && !in2.eof() && ch != '\n'; ch = in2.get())
		{
			str += ch;
		}
		dir = str[0];
		if (dir == 'L')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				x--; counter++;
				if (map1[y][x] < 'z')
				{
					distdata[map1[y][x]]; distdata[map1[y][x]].first = counter;
				}
			}
		}
		if (dir == 'R')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				x++; counter++;
				if (map1[y][x] < 'z')
				{
					distdata[map1[y][x]]; distdata[map1[y][x]].first = counter;
				}
			}
		}
		if (dir == 'U') // mirrored, down in the map space
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				y++; counter++;
				if (map1[y][x] < 'z')
				{
					distdata[map1[y][x]]; distdata[map1[y][x]].first = counter;
				}
			}
		}
		if (dir == 'D')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				y--; counter++;
				if (map1[y][x] < 'z')
				{
					distdata[map1[y][x]]; distdata[map1[y][x]].first = counter;
				}
			}
		}
	}

	//Traverse map2.
	x = fieldSize / 2;
	y = fieldSize / 2;
	ch = 0; dir = 0; dir_old = 0;
	counter = 0;
	while (!in2.eof() && ch != '\n')
	{

		std::string str;
		for (ch = in2.get(); ch != ',' && !in2.eof() && ch != '\n'; ch = in2.get())
		{
			str += ch;
		}
		dir = str[0];
		if (dir == 'L')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				x--; counter++;
				if (map2[y][x] < 'z')
				{
					distdata[map2[y][x]].second = counter;
				}
			}
		}
		if (dir == 'R')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				x++; counter++;
				if (map2[y][x] < 'z')
				{
					distdata[map2[y][x]].second = counter;
				}
			}
		}
		if (dir == 'U') // mirrored, down in the map space
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				y++; counter++;
				if (map2[y][x] < 'z')
				{
					distdata[map2[y][x]].second = counter;
				}
			}
		}
		if (dir == 'D')
		{
			int nSteps = std::stoi(str.erase(0, 1));
			for (int i = 0; i < nSteps; i++)
			{
				y--; counter++;
				if (map2[y][x] < 'z')
				{
					distdata[map2[y][x]].second = counter;
				}
			}
		}
	}

	std::vector<int> shortestPath;
	for (int i = 1; i <= (int)distdata.size(); i++)
	{
		std::cout << "Crossing " << i << ": " << "(" << distdata[i].first << "," << distdata[i].second << "), total steps: " << distdata[i].first + distdata[i].second << std::endl;
		shortestPath.push_back(distdata[i].first + distdata[i].second);
	}

	std::sort(shortestPath.begin(), shortestPath.end());
	std::cout << "Shortest paths:\n";
	for (int v : shortestPath)
	{
		std::cout << v << std::endl;
	}
}
void Day4()
{
	int start = 278384;
	int end = 824795;
	int count = 0;
	isValidPassword(788999);
	for (int num = start; num <= end; num++)
	{
		if (isValidPassword(num))
		{
			count++;
			std::cout << num << std::endl;
		}
	}
	std::cout << "TOTAL VALID: " << count;
}
void Day5()
{
	IntCode comp("day5input.txt");
	comp.Run(); // (supply code 1 as only input for 1st part of day1, code 5 for 2nd part)
	std::cout << "\nEnded.\n";
}
void Day6()
{
	std::vector<std::string> planets;// = { "A", "B","C","D","E","F","G","H" };
	std::map<std::string, std::string> map;

	std::ifstream in("day6input.txt");
	std::vector<int> mass;
	char ch = 0;
	while (!in.eof())
	{
		std::string str;
		std::string planetA;
		for (ch = in.get(); ch != '\n' && !in.eof(); ch = in.get())
		{
			if (ch != ')')
			{
				str += ch;
			}
			else
			{
				planetA = str;
				str = "";
			}
		}
		planets.push_back(str);
		map[str] = planetA;
		if (in.eof()) break;
	}

	int count = 0;
	for (auto p : planets)
	{
		count += nToRoot(p, map);
	}
	std::cout << "n = " << count;


	std::map<std::string, int> voyageYOU;
	std::map<std::string, int> voyageSAN;
	count = -1;
	for (std::string p = "YOU"; p != "COM"; p = map[p])
	{
		voyageYOU[p] = count++;
	}
	count = -1;
	for (std::string p = "SAN"; p != "COM"; p = map[p])
	{
		voyageSAN[p] = count++;
	}
	int minSteps = 0b1111111111111111111111111111111;
	for (auto p : voyageYOU)
	{
		for (auto q : voyageSAN)
		{
			if (p.first == q.first)
			{
				minSteps = min(minSteps, p.second + q.second);
			}
		}
	}
	std::cout << "\nminSteps = " << minSteps;
}
void Day7a()
{
	//Load Intcode computers for the Amps
	IntCode ampA("day7ainput.txt");
	IntCode ampB("day7ainput.txt");
	IntCode ampC("day7ainput.txt");
	IntCode ampD("day7ainput.txt");
	IntCode ampE("day7ainput.txt");

	// Generate all permutations of input signal and put in "options", a vector of strings 
	std::string input = "01234";
	std::vector<std::string> options;
	permute(options, input, "");

	// Check result for all possible options, log maxResult and maxOption everytime a new option yields a better result
	long long int maxResult = -1;
	std::string maxOption;
	for (auto str : options)
	{
		long long int resA = ampA.Run(str[0] - '0', 0)			; ampA.Reset();
		long long int resB = ampB.Run(str[1] - '0', resA)		; ampB.Reset();
		long long int resC = ampC.Run(str[2] - '0', resB)		; ampC.Reset();
		long long int resD = ampD.Run(str[3] - '0', resC)		; ampD.Reset();
		long long int tmpResult = ampE.Run(str[4] - '0', resD)	; ampE.Reset();
		
		if (tmpResult > maxResult)
		{
			maxResult = tmpResult;
			maxOption = str;
		}
		tmpResult = 0;
	}
	std::cout << "Result: " << maxResult << " with Signal: " << maxOption;

}
void Day7b()
{
	//Load Intcode computers for the Amps
	IntCode ampA("day7binput.txt");
	IntCode ampB("day7binput.txt");
	IntCode ampC("day7binput.txt");
	IntCode ampD("day7binput.txt");
	IntCode ampE("day7binput.txt");

	// Generate all permutations of input signal and put in "options", a vector of strings 
	std::string input = "56789";
	std::vector<std::string> options;
	permute(options, input, "");

	// Check result for all possible options, log maxResult and maxOption everytime a new option yields a better result
	long long int maxResult = -1;
	std::string maxOption;

	for (auto str : options)
	{
		long long int tmpResult = 0;
		// First run to set the phases
		long long int resA = ampA.Run(str[0] - '0',0);
		long long int resB = ampB.Run(str[1] - '0',resA);
		long long int resC = ampC.Run(str[2] - '0',resB);
		long long int resD = ampD.Run(str[3] - '0',resC);
		long long int resE = ampE.Run(str[4] - '0',resD);

		// loop until signal processing stops
		while (true)
		{
			resA = ampA.Run(resE); 
			if (resA == -1)
			{
				if (resE > maxResult)
				{
					maxResult = resE;
					maxOption = str;
				}
				ampA.Reset(); ampB.Reset(); ampC.Reset(); ampD.Reset(); ampE.Reset();
				break;
			}
			resB = ampB.Run(resA);
			resC = ampC.Run(resB);
			resD = ampD.Run(resC);
			resE = ampE.Run(resD);
		}
	}
	std::cout << "Result: " << maxResult << " with Signal: " << maxOption;

}
void Day8()
{
	std::ifstream in("day8ainput.txt");
	std::string str;
	in >> str;
	std::vector<char> pixelArray;
	for (char ch: str)
	{
		pixelArray.push_back(ch-'0');
	}

	int width = 25;
	int height = 6;
	int nPixelsPerLayer = width * height;
	int nLayers = int(pixelArray.size()) / (height * width);
	int min_0s = width;
	int multiple = 0;
	for (int layer = 0; layer < nLayers; layer++)
	{
		std::vector<int> counterVec = { 0,0,0 }; // counts occurances of 0,1,2 respectively
		for (int i = 0; i < nPixelsPerLayer; i++)
		{
			counterVec[pixelArray[nPixelsPerLayer * layer + i]]++;
		}
		if (counterVec[0] < min_0s)
		{
			min_0s = counterVec[0];
			multiple = counterVec[1] * counterVec[2];
		}
	}
	std::cout << "The multiple is: " << multiple<<std::endl << std::endl;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int layer = 0; layer < nLayers; layer++)
			{
				char cellColor = pixelArray[(y * width + x) + layer * nPixelsPerLayer]+'0';
				if (cellColor != '2')
				{
					std::cout << (cellColor == '1' ? "&": " ");
					break;
				}
			}
		}
		std::cout << std::endl;
	}
}
void Day9()
{
	IntCode comp("day9input.txt");
	comp.Run();
}
void Day10()
{
	// ##############################################################################
	// PART 1
	// ##############################################################################
	// Load astroid map
	std::ifstream in("day10input.txt");
	std::vector<Pos> astroids;
	char ch;
	int x = 0;
	int y = 0;
	for (ch = in.get(); !in.eof(); ch=in.get())
	{
		if (ch == '\n') // newline encountered
		{
			ch = in.get();
			x = 0;
			y++;
			std::cout << std::endl;
		}
		if (ch == '#') // astroid present at location
		{
			astroids.push_back({ x,y });
		}
		x++;
		std::cout << ch;
	}
	std::cout << std::endl;

	Pos bestPos = { 0,0 };
	int maxVisible = 0;
	for (Pos activePos : astroids)
	{
		std::set<std::pair<int, int>> set;
		for (Pos p : astroids)
		{
			Pos dir = { p.x - activePos.x, p.y - activePos.y };
			ReduceToLowestCommonDenom(dir);

			set.insert({ dir.x,dir.y });
		}
		std::cout << "Astroid at: (" << activePos.x << "," << activePos.y << ") : " << set.size() - 1<<std::endl;
		if (int(set.size()-1) > maxVisible)
		{
			maxVisible = (int)set.size()-1;
			bestPos = activePos;
		}
	}
	std::cout << "The best position is (" << bestPos.x << "," << bestPos.y << "), with " << maxVisible << " other astroids detected:\n";
	//
	// print to be implemented

	// ##############################################################################
	// PART 2
	// ##############################################################################
	// General strategy: 
	// 1. make vector of unique normalized directions from monitoring station to astroids in the angular order (top of screen=0deg, clockwise to 360deg)
	// 2. make a map with [keys]: all unique normalized directions, [values]: vector of all astroids on that direction, sorted by distance, closeby to far away
	// 3. loop through vector (1), removing each first astroid in vector (2) that maps to each direction, until all vetors in the map are empty.
	//
	// Let monitoring station be at bestPos, fill all helper containers
	// -> fill [[set_baseDir]] with normalized directions from station to all planets
	// -> fill [[vec_Dir]] with all distance vertices from station to all planets
	std::set<std::pair<int, int>> set_baseDir;
	std::vector<std::pair<int, int>> vec_Dir;
	std::vector<std::pair<int, int>> vec_baseDir;
	for (Pos p : astroids)
	{
		Pos dir = { p.x - bestPos.x, p.y - bestPos.y };
		vec_Dir.push_back({ dir.x,dir.y });
		ReduceToLowestCommonDenom(dir);
		set_baseDir.insert({ dir.x,dir.y });
		std::cout << "Planet (" << p.x << "," << p.y << ") has direction vector (" << dir.x << "," << dir.y << "), at angle: " << getClockAngle({ dir.x,dir.y }) << " deg\n";
	}
	// -> sort [[vec_Dir]] in order of distance to monitoring station (x+y) closeby to far
	//
	std::sort(vec_Dir.begin(), vec_Dir.end(), [](std::pair<int, int> p1, std::pair<int, int> p2) { return (p1.first+p1.second) > (p2.first+p2.second); });
	// -> fill [[vec_baseDir]], a vector with all normalized directions and sort 
	//    in order of their direction angle (0deg = top of screen, clockwise to 360 deg )
	for (auto p : set_baseDir) vec_baseDir.push_back(p);
	std::sort(vec_baseDir.begin(), vec_baseDir.end(), [](std::pair<int, int> p1, std::pair<int, int> p2) { return getClockAngle(p1) < getClockAngle(p2); });
	// -> fill [[map]], that maps each base direction to a vector that contains all planets in the line of sight for that base direction
	//
	std::map< std::pair<int,int>, std::vector<std::pair<int,int>> > map;
	for (auto d : vec_Dir)
	{
		Pos baseD = {d.first,d.second};
		ReduceToLowestCommonDenom(baseD);
		map[{baseD.x, baseD.y}].push_back({ d.first,d.second });
	}
	
	// Now, loop though [[vec_baseDir]] until all vectors in the map are empty, shooting the first element of each vector at every pass 
	// We'll fill a new vector [[vec_Destroyed]] to log the astroids in the order that they are destroyed
	std::vector<std::pair<int, int>> vec_Destroyed;
	int astroidsLeft = (int)vec_Dir.size();
	std::cout << "Destroying: ";
	while (astroidsLeft > 0)
	{
		for (auto d : vec_baseDir)
		{
			if (map[d].size() > 0)
			{
				int x_rel = map[d][0].first;
				int y_rel = map[d][0].second;
				if (x_rel != 0 || y_rel != 0) // own astroid at relative position (0,0) cannot be destroyed
				{
					vec_Destroyed.push_back(map[d][0]);
					std::cout << "(" << map[d][0].first << "," << map[d][0].second << "),";
				}
				map[d].erase(map[d].begin());
				astroidsLeft--;
			}
		}
	} std::cout << std::endl;
	// NOW WE KNOW THE ANSWER
	int nBet = 200; 
	int nDestroyed = (int)vec_Destroyed.size(); if (nDestroyed < nBet) nBet = nDestroyed;
	int x_abs = vec_Destroyed[nBet-1].first + bestPos.x;
	int y_abs = vec_Destroyed[nBet-1].second + bestPos.y;
	std::cout << "The "<<nBet<<"th astroid destroyed is at absolute position: (" << x_abs<< "," << y_abs << ").\n";
	std::cout << "The answer (100*x+y) = " << 100 * x_abs + y_abs << "\n";
}
void Day11()
{
	// 0=black, 1=white
	int fieldWidth = 120;
	int fieldHeight = 110;
	int nPanels = fieldWidth * fieldHeight;
	std::map<Direction, std::pair<int, int>> directionMap;
	directionMap[Direction::UP]		= { 0,-1 };
	directionMap[Direction::RIGHT]	= { 1, 0 };
	directionMap[Direction::DOWN]	= { 0, 1 };
	directionMap[Direction::LEFT]	= {-1, 0 };

	IntCode comp("day11input.txt");
	std::pair<int, int> pos = { fieldWidth/2-5, fieldHeight/2 +20};
	std::vector<int> field(nPanels);
	std::vector<int> field_copy(nPanels);
	std::set<std::pair<int, int>> paintedPositions;
	int currentColor = 1;
	Direction dir = Direction::UP;
	int minx=fieldWidth/2;
	int maxx=0;
	int miny=fieldHeight/2;
	int maxy=0;

	while (true)
	{
		int instr0 = (int)comp.Run(currentColor); if (instr0 == -1) break;
		int instr1 = (int)comp.Run(0); if (instr1 == -1) break;
		//std::cout << "Current panel: (" << pos.first << "," << pos.second << "), ";
		//std::cout << "Current color: " << currentColor<<", ";
		//std::cout << "Instruction received: [" << instr0 << "," << instr1 << "], ";
		
		// apply paint
		field[pos.second * fieldWidth + pos.first] = instr0;
		field_copy[pos.second * fieldWidth + pos.first] = 1;
		paintedPositions.emplace(pos);

		// adjust direction (there has got to be a better way, but this works)
		(instr1 == 1) ? (dir = Direction((int(dir) + 1) % int(Direction::count))) : (dir = Direction((int(dir) - 1 + int(Direction::count)) % int(Direction::count)));
		// take step
		pos.first += directionMap[dir].first;
		pos.second += directionMap[dir].second;
		// read current color
		currentColor = field[pos.second * fieldWidth + pos.first];
		// output new position
		minx = min(minx, pos.first);
		miny = min(miny, pos.second);
		maxx = max(maxx, pos.first);
		maxy = max(maxy, pos.second);
		//std::cout<<"New position: ("<< pos.first << "," << pos.second << "). \n";
	}
	std::cout << "\nCode ended.\n";

	//printing the canvas
	for (int y = 0; y < fieldHeight; y++)
	{
		for (int x = 0; x < fieldWidth; x++)
		{
			if ((y * fieldWidth + x) < nPanels)
			{
				if (field[y * fieldWidth + x] == 1) std::cout << "#"; else std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Minimum number of painted positions: " << paintedPositions.size();
	std::cout << "\nxmin: " << minx << " xmax: " << maxx << "\n";
	std::cout << "ymin: " << miny << " ymax: " << maxy << "\n";
}
void Day12a()
{
	PlanetSys pSys;
	pSys.AddPlanet({ -6, -5,  -8});
	pSys.AddPlanet({  0, -3, -13});
	pSys.AddPlanet({-15, 10, -11});
	pSys.AddPlanet({ -3, -8,   3});
	for (int i = 0; i < 1000; i++)
	{
		pSys.Update();
	}
	pSys.PrintCoordinates();
}
void Day12b()
{
	PlanetSys planetSys;
	//planetSys.AddPlanet({ -1,  0, 2 });
	//planetSys.AddPlanet({  2,-10,-7 });
	//planetSys.AddPlanet({  4, -8, 8 });
	//planetSys.AddPlanet({  3,  5,-1 });

	//planetSys.AddPlanet({ -8, -10,  0 });
	//planetSys.AddPlanet({ 5, 5, 10 });
	//planetSys.AddPlanet({ 2, -7, 3 });
	//planetSys.AddPlanet({ 9, -8,   -3 });

	planetSys.AddPlanet({ -6, -5,  -8 });
	planetSys.AddPlanet({  0, -3, -13 });
	planetSys.AddPlanet({-15, 10, -11 });
	planetSys.AddPlanet({ -3, -8,   3 });
	while (true)
	{
		planetSys.Update();
		planetSys.Print2D();
	}

	{
		std::vector < long long int> orbResPeriods = planetSys.GetOrbitalResonancePeriods();
		unsigned long long int pAll = std::lcm(orbResPeriods[0], std::lcm(orbResPeriods[1],orbResPeriods[2]));
		std::cout << "ORP for planet system: " << pAll <<std::endl;
	}
}
void Day13()
{
	IntCode arcadeComputer("day13input.txt");

	int fieldWidth = 45;
	int fieldHeight = 32;
	int nPanels = fieldWidth * fieldHeight;
	std::vector<int> field(nPanels);
	bool GameEnd = false;

	while (true)
	{
		for (int i = 0; i < 3; i++)
		{
			int x = arcadeComputer.Run(-10);
			if (x == -999)
			{
				GameEnd = true;
				break;
			}
			int y = arcadeComputer.Run(0);
			int id = arcadeComputer.Run(0);
			if (x == -1)
			{
				arcadeComputer.PrintTextToConsole("Score= ", { 0,31 });
				std::cout << id;
			}
			else
			{
				field[y * fieldWidth + x] = id;
				arcadeComputer.PrintSymbolToConsole(id, { (SHORT)x,(SHORT)y });
			}
		}
		if (GameEnd) break;
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	int count = 0;
	for (int v : field)
	{
		if (v == 2) count++;
	}
	arcadeComputer.PrintTextToConsole("Blocks in field: ", { 0,32 });
	std::cout<< count;
}
struct R {
	R(std::string str, int n)
		:
		element(str),
		n(n)
	{}
	friend bool operator==(const R& lhs, const R& rhs)
	{
		return (lhs.element == rhs.element && lhs.n == rhs.n);
	}
	friend bool operator!=(const R& lhs, const R& rhs)
	{
		return (lhs.element != rhs.element || lhs.n != rhs.n);
	}
	friend bool operator<(const R& lhs, const R& rhs)
	{
		return lhs.element < rhs.element;
	}
	std::string element;
	int n;
};
struct ProdTables {
	std::map<std::string, int>& MinProduction;
	std::map<std::string, int>& Stock;
	std::map<std::string, std::vector<R>>& ProductionMap;
};
int OreRequired(std::string product, int nRequired, ProdTables& tables)
{
	if (nRequired <= 0) return 0;
	{	// Manage stock (first use up what's in stock)
		int inStock = tables.Stock[product];
		if (inStock > 0)
		{
			if (inStock >= nRequired)
			{
				tables.Stock[product] -= nRequired;
				return 0;
			}
			else
			{
				nRequired -= inStock;
				tables.Stock[product] -= 0;
			}
		}
	}
	
	int sum = 0;
	int minprod = tables.MinProduction[product];
	int quant = 0;
	if (minprod >= nRequired) quant = 1; else quant = std::ceil(float(nRequired) / float(minprod));
	int overproduction = quant * minprod - nRequired;

	for (R r : tables.ProductionMap[product])
	{
		if (r.element == "ORE")
		{
			tables.Stock[product] += overproduction;
			std::cout << "ORE used: " << r.n * quant<<std::endl;
			return r.n * quant;
		}
		else
		{
			sum += OreRequired(r.element, quant * r.n, tables);
			tables.Stock[r.element] += overproduction;
		}
	}
	return sum;
}

void Day14()
{
	std::map<std::string, int> MinProduction;
	std::map<std::string, int> Stock;
	std::map<std::string, std::vector<R>> ProductionMap;

	MinProduction["FUEL"] = 1;
	MinProduction["CA"] = 1;
	MinProduction["BC"] = 1;
	MinProduction["AB"] = 1;
	MinProduction["C"] = 5;
	MinProduction["B"] = 3;
	MinProduction["A"] = 2;

	Stock["FUEL"] = 0;
	Stock["CA"] = 0;
	Stock["BC"] = 0;
	Stock["AB"] = 0;
	Stock["C"] = 0;
	Stock["B"] = 0;
	Stock["A"] = 0;

	ProductionMap["FUEL"] = { {"CA",4},{"BC",3},{"AB",2} };
	ProductionMap["CA"] = { {"C",4},{"A",1} };
	ProductionMap["BC"] = { {"B",5},{"C",7} };
	ProductionMap["AB"] = { {"A",3},{"B",4} };
	ProductionMap["C"] = { {"ORE",7}, };
	ProductionMap["B"] = { {"ORE",8} };
	ProductionMap["A"] = { {"ORE",9} };

	ProdTables tables = { MinProduction,Stock,ProductionMap };
	//R start = R{ "C",6 };
	//R start = R{ "FUEL",1 };
	int k = OreRequired("FUEL",1, tables);
	std::cout << "Ore required: " << k << std::endl;
	std::cout << "Stock:\n";
	for (auto e : Stock)  std::cout << e.first << ":" << e.second << std::endl;
}
	


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day14();

	while (!_kbhit());
}