#pragma once
#include "IncludesUsed.h"

void Day1()
{
	std::ifstream in("Resources/day1input.txt");
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
	IntCode comp("Resources/day2input.txt");
	comp.GetRunCodeVectorReference()[1] = 12;
	comp.GetRunCodeVectorReference()[2] = 2;
	comp.Run();
	std::cout << "Answer to part 1: " << comp.GetRunCodeVectorReference()[0];
}
void Day2b()
{
	IntCode comp("Resources/day2input.txt");

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

	std::ifstream in("Resources/day3input.txt");
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
	std::ifstream in2("Resources/day3input.txt");
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
	IntCode comp("Resources/day5input.txt");
	comp.Run(); // (supply code 1 as only input for 1st part of day1, code 5 for 2nd part)
	std::cout << "\nEnded.\n";
}
void Day6()
{
	std::vector<std::string> planets;// = { "A", "B","C","D","E","F","G","H" };
	std::map<std::string, std::string> map;

	std::ifstream in("Resources/day6input.txt");
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
	IntCode ampA("Resources/day7ainput.txt");
	IntCode ampB("Resources/day7ainput.txt");
	IntCode ampC("Resources/day7ainput.txt");
	IntCode ampD("Resources/day7ainput.txt");
	IntCode ampE("Resources/day7ainput.txt");

	// Generate all permutations of input signal and put in "options", a vector of strings 
	std::string input = "01234";
	std::vector<std::string> options;
	permute(options, input, "");

	// Check result for all possible options, log maxResult and maxOption everytime a new option yields a better result
	long long int maxResult = -1;
	std::string maxOption;
	for (auto str : options)
	{
		long long int resA = ampA.Run<long long>(str[0] - '0', 0); ampA.Reset();
		long long int resB = ampB.Run<long long>(str[1] - '0', resA); ampB.Reset();
		long long int resC = ampC.Run<long long>(str[2] - '0', resB); ampC.Reset();
		long long int resD = ampD.Run<long long>(str[3] - '0', resC); ampD.Reset();
		long long int tmpResult = ampE.Run<long long>(str[4] - '0', resD); ampE.Reset();

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
	IntCode ampA("Resources/day7binput.txt");
	IntCode ampB("Resources/day7binput.txt");
	IntCode ampC("Resources/day7binput.txt");
	IntCode ampD("Resources/day7binput.txt");
	IntCode ampE("Resources/day7binput.txt");

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
		long long int resA = ampA.Run<long long>(str[0] - '0', 0);
		long long int resB = ampB.Run<long long>(str[1] - '0', resA);
		long long int resC = ampC.Run<long long>(str[2] - '0', resB);
		long long int resD = ampD.Run<long long>(str[3] - '0', resC);
		long long int resE = ampE.Run<long long>(str[4] - '0', resD);

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
	std::ifstream in("Resources/day8ainput.txt");
	std::string str;
	in >> str;
	std::vector<char> pixelArray;
	for (char ch : str)
	{
		pixelArray.push_back(ch - '0');
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
	std::cout << "The multiple is: " << multiple << std::endl << std::endl;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int layer = 0; layer < nLayers; layer++)
			{
				char cellColor = pixelArray[(y * width + x) + layer * nPixelsPerLayer] + '0';
				if (cellColor != '2')
				{
					std::cout << (cellColor == '1' ? "&" : " ");
					break;
				}
			}
		}
		std::cout << std::endl;
	}
}
void Day9()
{
	IntCode comp("Resources/day9input.txt");
	comp.Run();
}
void Day10()
{
	// ##############################################################################
	// PART 1
	// ##############################################################################
	// Load astroid map
	std::ifstream in("Resources/day10input.txt");
	std::vector<Pos> astroids;
	char ch;
	int x = 0;
	int y = 0;
	for (ch = in.get(); !in.eof(); ch = in.get())
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
		std::cout << "Astroid at: (" << activePos.x << "," << activePos.y << ") : " << set.size() - 1 << std::endl;
		if (int(set.size() - 1) > maxVisible)
		{
			maxVisible = (int)set.size() - 1;
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
	std::sort(vec_Dir.begin(), vec_Dir.end(), [](std::pair<int, int> p1, std::pair<int, int> p2) { return (p1.first + p1.second) > (p2.first + p2.second); });
	// -> fill [[vec_baseDir]], a vector with all normalized directions and sort 
	//    in order of their direction angle (0deg = top of screen, clockwise to 360 deg )
	for (auto p : set_baseDir) vec_baseDir.push_back(p);
	std::sort(vec_baseDir.begin(), vec_baseDir.end(), [](std::pair<int, int> p1, std::pair<int, int> p2) { return getClockAngle(p1) < getClockAngle(p2); });
	// -> fill [[map]], that maps each base direction to a vector that contains all planets in the line of sight for that base direction
	//
	std::map< std::pair<int, int>, std::vector<std::pair<int, int>> > map;
	for (auto d : vec_Dir)
	{
		Pos baseD = { d.first,d.second };
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
	int x_abs = vec_Destroyed[nBet - 1].first + bestPos.x;
	int y_abs = vec_Destroyed[nBet - 1].second + bestPos.y;
	std::cout << "The " << nBet << "th astroid destroyed is at absolute position: (" << x_abs << "," << y_abs << ").\n";
	std::cout << "The answer (100*x+y) = " << 100 * x_abs + y_abs << "\n";
}
void Day11()
{
	// 0=black, 1=white
	int fieldWidth = 120;
	int fieldHeight = 110;
	int nPanels = fieldWidth * fieldHeight;
	std::map<Direction, std::pair<int, int>> directionMap;
	directionMap[Direction::UP] = { 0,-1 };
	directionMap[Direction::RIGHT] = { 1, 0 };
	directionMap[Direction::DOWN] = { 0, 1 };
	directionMap[Direction::LEFT] = { -1, 0 };

	IntCode comp("Resources/day11input.txt");
	std::pair<int, int> pos = { fieldWidth / 2 - 5, fieldHeight / 2 + 20 };
	std::vector<int> field(nPanels);
	std::vector<int> field_copy(nPanels);
	std::set<std::pair<int, int>> paintedPositions;
	int currentColor = 1;
	Direction dir = Direction::UP;
	int minx = fieldWidth / 2;
	int maxx = 0;
	int miny = fieldHeight / 2;
	int maxy = 0;

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
	pSys.AddPlanet({ -6, -5,  -8 });
	pSys.AddPlanet({ 0, -3, -13 });
	pSys.AddPlanet({ -15, 10, -11 });
	pSys.AddPlanet({ -3, -8,   3 });
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
	planetSys.AddPlanet({ 0, -3, -13 });
	planetSys.AddPlanet({ -15, 10, -11 });
	planetSys.AddPlanet({ -3, -8,   3 });
	while (true)
	{
		planetSys.Update();
		planetSys.Print2D();
	}

	{
		std::vector < long long int> orbResPeriods = planetSys.GetOrbitalResonancePeriods();
		unsigned long long int pAll = std::lcm(orbResPeriods[0], std::lcm(orbResPeriods[1], orbResPeriods[2]));
		std::cout << "ORP for planet system: " << pAll << std::endl;
	}
}
void Day13()
{
	IntCode arcadeComputer("Resources/day13input.txt");

	int fieldWidth = 45;
	int fieldHeight = 32;
	int nPanels = fieldWidth * fieldHeight;
	std::vector<int> field(nPanels);
	bool GameEnd = false;

	while (true)
	{
		for (int i = 0; i < 3; i++)
		{
			int x = (int)arcadeComputer.Run(-10);
			if (x == -999)
			{
				GameEnd = true;
				break;
			}
			int y = (int)arcadeComputer.Run(0);
			int id = (int)arcadeComputer.Run(0);
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
	std::cout << count;
}
void Day14()
{
	//======================================================================================
	// PART 1
	//======================================================================================
	std::map<std::string, long long int> MinProduction;
	std::map<std::string, std::vector<R>> ProductionMap;
	std::map<std::string, ChemNode*> ChemNodes;
	std::map<std::string, long long int> OreConvertingChemicals;
	// File Loading to fill data structures
	{
		std::ifstream in("Resources/day14input.txt");
		char ch;
		std::stack<std::string> lhs_values;
		std::stack<std::string> lhs_elements;
		std::string rhs_value;
		std::string rhs_element;

		bool switch_lineSide = 0;
		bool switch_pairSide = 0;
		while (!in.eof())
		{
			std::string str;
			for (ch = in.get(); ch != ',' && ch != ' ' && ch != '>' && ch != '=' && !in.eof() && ch != '\n'; ch = in.get())
			{
				str += ch;
			}

			if (ch == '=') {
				switch_lineSide = 1;
			}
			if (ch == '\n' || in.eof()) {
				rhs_element = str;
				MinProduction[rhs_element] = std::stoi(rhs_value);
				//Stock[rhs_element] = 0;
				while (lhs_elements.size() != 0)
				{
					std::string e = lhs_elements.top(); lhs_elements.pop();
					int n = std::stoi(lhs_values.top()); lhs_values.pop();
					ProductionMap[rhs_element].push_back({ e,n });
				}
				switch_lineSide = 0;
			}
			if (ch == ',' || ch == ' ')
			{
				if (switch_lineSide == 0 && str != "")
				{
					if (switch_pairSide == 0)
					{
						lhs_values.push(str);
						switch_pairSide = 1;
					}
					else
					{
						lhs_elements.push(str);
						switch_pairSide = 0;
					}
				}
				if (switch_lineSide == 1 && str != "")
				{
					rhs_value = str;
				}
			}
		}
		MinProduction["ORE"] = 1;
	}
	// END of file loading

	// Cluster containers in one struct
	ProdTables tables = { MinProduction,ProductionMap,ChemNodes,OreConvertingChemicals };

	//Build tree (rather: a naive implementation of a graph, since branches can interconnect)
	ChemNode tree("FUEL", tables);
	//Populate all nodes in the tree with production data (basically: run all purchase orders)
	tree.Purchase(1, tables);
	//Calculate total amount of Ore (tranversing to all "bottom leaves" Ore nodes)
	long long int k = tree.GetOreConsumption();
	std::cout << "Ore required for 1 FUEL: " << k << std::endl;
	std::cout << "Press return to continue to part 2\n";
	std::cin.get();

	//======================================================================================
	// PART 2
	//======================================================================================
	// Brute force the solution to part 2 (no time..)
	// It's embarrasing but it works
	long long int OreAvailable = (long long int)1E12;
	long long int fuel = 1;
	long long int OreRequired = 0;
	// First find threshold value (trial through exponential growth)
	while (OreRequired < OreAvailable)
	{
		fuel *= 2;
		tree.Clear();
		tree.Purchase(fuel, tables);
		OreRequired = tree.GetOreConsumption();
		std::cout << "Ore required for " << fuel << " FUEL: " << OreRequired << std::endl;
	}
	std::cout << "#####################################\n";
	fuel /= 2;
	OreRequired = 0;
	// Now iterate with decreasing linear intervals
	int fact = (int)std::pow(10, nDigits(fuel) - 1);
	while (fact >= 1)
	{
		while (OreRequired < OreAvailable)
		{
			tree.Clear();
			tree.Purchase(fuel, tables);
			OreRequired = tree.GetOreConsumption();
			std::cout << "Ore required for " << fuel << " FUEL: " << OreRequired << std::endl;
			fuel += fact;
		}
		std::cout << "#####################################\n";
		fuel -= 2 * fact;
		OreRequired = 0;
		fact /= 10;
	}
	std::cout << "#####################################\n";
}
bool MazeSolutionExists(Maze& maze, IntCode& mazeComputer, int lastDir)
{
	//Recursive backtracking algo, used to pathtrace in maze for Day15 part 1
	maze.stackDepth++;
	for (int dir = 1; dir < 5; dir++)
	{
		int computerOutput = -999;
		maze.ExploreCurrentPosition(mazeComputer);

		if (maze.totalsteps % 5 == 0)
		{
			maze.PrintMaze(true);
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
				maze.steps++; maze.totalsteps++;
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
				maze.steps++; maze.totalsteps++;
				maze.PrintMaze(true);
				maze.PrintTextToConsole("success! ", { 0,8 });
				std::cout << "End position: (" << maze.pos.x << "," << maze.pos.y << "); steps: " << maze.steps << "; totalsteps: " << maze.totalsteps << "; totalacksteps: " << maze.totalbacksteps << "; Stack depth: " << maze.stackDepth;
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
			maze.totalbacksteps++;
			return false;
		}

	}
	maze.PrintTextToConsole("error stack empty", { 0,8 }); //std::cout << "Error";
	return false;
}
void Day15a_RNG()
{
	// Approach: make random moves though the maze until you find the target
	// (very inefficient but it works. Number of moves will differ between runs

	// Initialization
	IntCode mazeComputer("Resources/day15input.txt");
	std::random_device rn;
	std::mt19937 rng(rn());
	std::uniform_int_distribution<int> distr(1, 4);
	std::vector<int> instructions;
	int response = 0;

	// Random search until you find target (computer response 2 means target is found)
	while (response != 2)
	{
		int move = distr(rng);

		response = mazeComputer.Run15(move);
		if (response == 1 || response == 2) instructions.push_back(move);
		if (instructions.size() % 100000 == 0)
		{
			std::cout << "Size of instructions vector: " << instructions.size() << std::endl;
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

	// METHOD 1: 
	// Remove all backtracking paths from positions (a vector of all (x,y) positions traversed)
	// Using a loop with (int i instead of iterator) because we need to remove elements from both
	// the instructions and positions vectors
	auto positions_copy = positions;
	size_t len = positions_copy.size();

	for (size_t i = 0; i < len; i++)
	{
		for (size_t j = i + 1; j < len; j++)
		{
			if (positions_copy[j] == positions_copy[i])
			{
				positions_copy.erase(positions_copy.begin() + i + 1, positions_copy.begin() + j + 1);
				instructions.erase(instructions.begin() + i + 1, instructions.begin() + j + 1);
				len = positions_copy.size();
				i--;
				break;
			}
		}
	}
	std::cout << "Method 1, n=" << positions_copy.size() << std::endl;

	// METHOD 2:
	// Remove all backtracking paths from positions (a vector of all (x,y) positions traversed) using iterators
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
	std::cout << "Method 2, n=" << positions.size() << std::endl;

	// Save instructions vector to file "Resources/maze_instr.bin"
	// first: write size of vector, then write all elements
	{
		std::ofstream out("Resources/maze_instr.bin", std::ios::binary);
		int vecsize = (int)instructions.size();
		out.write(reinterpret_cast<char*>(&vecsize), sizeof vecsize);
		for (int v : instructions)
		{
			out.write(reinterpret_cast<char*>(&v), sizeof v);
		}
		out.close();
		std::cout << "Instructions vector saved to 'maze_instr.bin'" << std::endl;
	}
	return;

}
void Day15a_ApplyPath()
{
	// This function applies the instruction set derived in Day15a_RNG() to visualize the maze
	// Used to debug & understand the maze layout
	// Day15a_RNG() should be run first to write data to maze_instr.bin
	IntCode mazeComputer("Resources/day15input.txt");
	Maze maze(20000, 45000, false, "nofile");
	maze.PrintMaze(false);

	{
		std::vector<int> instructions;
		std::ifstream in("Resources/maze_instr.bin", std::ios::binary);
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
			maze.PrintMaze(false);
		}
		maze.PrintMaze(false);
	}

}
void Day15a_Simulated()
{
	// this runs on a small custom made maze to test the backtracking algo 
	// (no int computer involved to make debugging easier)
	IntCode mazeComputer("Resources/day15input.txt");
	//Maze maze(20000, 45000);
	//maze.PrintMovingCenter();

	Maze maze(25, 8, true, "Resources/day15test.txt");
	maze.PrintMaze(true);
	MazeSolutionExists(maze, mazeComputer, 4);
	return;
}
void Day15a()
{
	IntCode mazeComputer("Resources/day15input.txt");
	Maze maze(72, 72, false, "nofile");
	MazeSolutionExists(maze, mazeComputer, 4);

	// Write field vector of maze to "Resources/maze_layout.bin", binary mode
	/*
	{
		std::ofstream out("Resources/maze_layout.bin", std::ios::binary);
		int vecsize = maze.field.size();
		out.write(reinterpret_cast<char*>(&vecsize), sizeof vecsize);
		for (char c : maze.field)
		{
			out.write(reinterpret_cast<char*>(&c), sizeof c);
		}
		out.close();
	}
	*/

	// Write field vector of maze to "Resources/maze_layout.txt", text mode
	/*
	{
		std::ofstream out2("Resources/maze_layout2.txt", std::ios::out);
		int count = 0;
		for (char c : maze.field)
		{
			out2<<c;
			count++;
			if (count % 72 == 0) out2 << '\n';
		}
		out2.close();
	}
	*/
	return;
}
void Day15b()
{
	// Solution can be made more efficient, but this will do

	Maze maze(41, 41, true, "Resources/maze_layout.txt"); //Load the maze as found on Part 1 of Day15

	// Initialize
	std::set<std::pair<int, int>> set;
	std::set<std::pair<int, int>> set_local;
	int count = 0;
	int size_start = 0;
	set.insert({ maze.pos.x, maze.pos.y });
	int size_end = 1;

	// Each loop: add adjacent rooms (adjacent to all rooms in the set that have already been filled) to the set
	// until the set doesn't grow anymore (all rooms filled)
	while (true)
	{
		size_start = (int)set.size();
		for (auto p : set)
		{
			for (int dir = 1; dir < 5; dir++)
			{
				if (maze.AvailableInTestField({ p.first,p.second }, dir))
				{
					Pos nextpos = AddDirToPos(dir, { p.first,p.second });
					set_local.insert({ nextpos.x,nextpos.y });
				}
			}
		}
		for (auto p_local : set_local)
		{
			set.insert(p_local);
		}
		size_end = (int)set.size();
		if (size_end == size_start) break; //all rooms are filled
		count++;
	}
	std::cout << "Minutes required: " << count;

}
void Day16a()
{
	// Load data from file
	std::vector<int> signal;
	{
		std::ifstream in("Resources/day16input.txt");
		char ch;
		for (in >> ch; !in.eof(); in >> ch)
		{
			signal.push_back(int(ch - '0'));
		}
	}

	// Initialization
	std::vector<int> basic_pattern = { 0,1,0,-1 };
	std::vector<std::vector<int>> maskMatrix = FFTBuildMaskMatrix(basic_pattern, (int)signal.size());

	// Apply FFT 100 times
	std::vector<int> result = FFT(signal, maskMatrix, 100);
	std::cout << "The first eight digits of the result: ";
	for (int i = 0; i < 8; i++) std::cout << result[i];
}
void Day16b()
{
	// Load data from file
	std::vector<int> signal_orig;
	int offset = 0;
	{
		std::ifstream in("Resources/day16input.txt");
		int count = 0;
		char ch;
		for (in >> ch; !in.eof(); in >> ch)
		{
			signal_orig.push_back(int(ch - '0'));
			if (count < 7)
			{
				offset += int(std::pow(10, 7 - count - 1)) * int(ch - '0');
				count++;
			}
		}
	}
	// Fill signal vetor x10.000
	std::vector<int> signal_full;
	for (int i = 0; i < 10000; i++)
	{
		std::copy(signal_orig.begin(), signal_orig.end(), std::back_inserter(signal_full));
	}
	std::vector<int> sub_signal(signal_full.begin() + offset, signal_full.end());
	size_t sub_signalLength = sub_signal.size();

	// FFT on sub-signal
	std::vector<int> temp;
	for (int n = 0; n < 100; n++)
	{
		temp.clear();
		temp.push_back(sub_signal.back());
		for (size_t i = sub_signalLength - 1; i > 0; i--)
		{
			int elem = temp.back() + sub_signal[i - 1];
			temp.push_back(elem % 10);
		}
		std::reverse(temp.begin(), temp.end());
		sub_signal.swap(temp);
	}
	std::cout << "The first eight digits of the result: ";
	for (int i = 0; i < 8; i++) std::cout << sub_signal[i];
}
void Day17()
{
	//Field field; // field is printed and calibration value shown upon initialization (construction)

	// Upon manual inspection of the field and with quite manageable constraints, we deduce:
	//   Main routine: [A,B,C,A,B,C,B,C,A]
	//    Function A: [L,12,R,4,R,4,L,6]
	//    Function B: [L,12,R,4,R,4,R,12]
	//    Function C: [L,10,L,6,R,4]
	// However, step instructions only run up to 9, so we rearrange into:
	//   Main routine: [A,B,C,A,B,C,B,C,A]
	//    Function A: [L,6,6,R,4,R,4,L,6]
	//    Function B: [L,6,6,R,4,R,4,R,6,6]
	//    Function C: [L,5,5,L,6,R,4]



	// Compose full string of instructions
	// (Note that the C++ string type is an array of chars, so:
	//		std::string a = "a\nb";
	//		char b = a[1];
	// Will yield b='\n' (or: b=10). Type casting between char and int is trivial
	std::vector<char> Main = { 'A',44,'B',44,'A',44,'C',44,'A',44,'B',44,'C',44,'B',44,'C',44,'A', 10 };
	std::vector<char> FuncA = { 'L',44,6 + '0',44,6 + '0',44,'R',44,4 + '0',44,'R',44,4 + '0',44,'L',44,6 + '0',		10 };
	std::vector<char> FuncB = { 'L',44,6 + '0',44,6 + '0',44,'R',44,4 + '0',44,'R',44,4 + '0',44,'R',44,6 + '0',44,6 + '0',	10 };
	std::vector<char> FuncC = { 'L',44,5 + '0',44,5 + '0',44,'L',44,6 + '0',44,'R',44,4 + '0', 10 };
	std::vector<char> Other = { 'n',							10 };
	std::vector<char> FullInstructions;
	FullInstructions.insert(FullInstructions.end(), Main.begin(), Main.end());
	FullInstructions.insert(FullInstructions.end(), FuncA.begin(), FuncA.end());
	FullInstructions.insert(FullInstructions.end(), FuncB.begin(), FuncB.end());
	FullInstructions.insert(FullInstructions.end(), FuncC.begin(), FuncC.end());
	FullInstructions.insert(FullInstructions.end(), Other.begin(), Other.end());

	// Create new instance of intcode
	IntCode ASCIIcomp("Resources/day17input.txt");
	ASCIIcomp.SetFirstDigit(2);
	std::cout << "\n\nDust cleaned: \n" << ASCIIcomp.Run(FullInstructions);
	//ASCIIcomp.Run();



}
void Day18a()
{
	// Init global data structs===========
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::map<char, int> keyIndices;
	std::map<char, Coi2> keyFieldCoordinates;
	// Load the field & ------------------
	Coi2 pos0 = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys, keyIndices, keyFieldCoordinates);
	PrintField(field, fieldWidth);
	// Initialize for BFS ----------------
	KeyPosition P0 = { {'@',keyFieldCoordinates['@']},"@",0,"@" };
	std::set<std::string> visitedKeys;
	std::queue<KeyPosition> keyQueue;
	keyQueue.push(P0);
	std::map<std::string, CacheInfo> cache;
	DiagnosticData diagData;
	// End Init ==========================

	// Main BFS loop
	std::vector<std::pair<std::string, int>> result;
	while (!keyQueue.empty())
	{
		if (keyQueue.size() > diagData.MaxQueueSize_OuterBFS) diagData.MaxQueueSize_OuterBFS = keyQueue.size();
		diagData.loopCount_OuterBFS++;
		KeyPosition curKeyPos = keyQueue.front(); keyQueue.pop();
		std::vector<std::pair<Key, int>> newKeys = GetAvailableKeyPositions(curKeyPos, field, fieldWidth, visitedKeys, cache, diagData);

		std::vector<KeyPosition> newPositions;
		for (std::pair<Key, int> p : newKeys)
		{
			std::string newkeys = curKeyPos.keys + p.first.keychar;
			std::sort(newkeys.begin(), newkeys.end());
			KeyPosition newKeyPosition = { p.first , newkeys, curKeyPos.steps + p.second, curKeyPos.path + p.first.keychar };
			newPositions.push_back(newKeyPosition);
		}

		for (KeyPosition p : newPositions)
		{
			if (p.keys.size() == nKeys) // end condition
			{
				//std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
				result.push_back({ p.path,p.steps });
			}
			else
			{
				visitedKeys.insert(Hash(p));
				keyQueue.push(p);
			}
		}
	}

	// Display Results:
	std::cout << "\n\nResults found.\n";
	std::cout << "==============\n";
	std::cout << "Number of keys found: " << nKeys << '\n';
	std::cout << "Number of unique routes: " << result.size() << '\n';

	std::vector<std::pair<std::string, int>>::iterator max_it = std::max_element(result.begin(), result.end(), route_compare);
	std::vector<std::pair<std::string, int>>::iterator min_it = std::min_element(result.begin(), result.end(), route_compare);
	std::cout << "Range of route lengths found: [" << min_it->second << "..." << max_it->second << "]\n";
	std::cout << "Example of shortest path: " << min_it->first << '\n';
	std::cout << "Example of longest path : " << max_it->first << '\n';

	std::cout << "\nAlgorithm diagnostics: \n";
	std::cout << "====================== \n";
	std::cout << "Outer BFS loop (key-to-key): \n";
	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_OuterBFS << '\n';
	std::cout << "> Loop count: " << diagData.loopCount_OuterBFS << '\n';
	std::cout << "Inner BFS loop (cell-to-cell): \n";
	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_InnerBFS << '\n';
	std::cout << "> Loop count: " << diagData.loopCount_InnerBFS << '\n';
	std::cout << "\nSize of key-to-key cache (#nodes in keyGraph): " << cache.size();

	//std::cout << "\n(press Enter to show keyGraph adjacency list)\n\n";
	//std::cin.get();
	std::cout << "\n\nAdjacency list (Node name is keylocation + keys_owned)\n";
	std::cout << "NODE         ARCS\n";
	std::cout << "======================================================\n";
	for (auto e : cache)
	{
		std::cout << std::left << std::setw(nKeys + 2) << e.first;
		std::cout << "->  ";
		for (auto p : e.second.cachedAvailablePositions)
		{
			std::cout << p.first.keychar << "[" << p.second << "], ";
		}
		std::cout << '\n';
	}
	return;
}
void Day18b()
{
	// Init global data structs===========
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::map<char, int> keyIndices;
	std::map<char, Coi2> keyFieldCoordinates;
	std::vector<std::vector<char>> subFields; // 0 (NW), 1 (NE), 2(SW), 3(SE)
	int subFieldWidth;
	// Load the field & ------------------
	std::vector<Coi2> pos0_vec = { }; // in local (subfield) coordinates
	pos0_vec = LoadField4D("Resources/day18binput.txt", field, fieldWidth, nKeys, keyIndices, keyFieldCoordinates, subFields, subFieldWidth);

	// Initialize for BFS ----------------
	KeyPosition4D P0 = { {{'1',pos0_vec[0]},{'2',pos0_vec[1]},{'3',pos0_vec[2]},{'4',pos0_vec[3]}},"@",0,"@" };
	std::set<std::string> visitedKeys;
	std::queue<KeyPosition4D> keyQueue;
	keyQueue.push(P0);
	std::map<std::string, CacheInfo4D> cache;
	DiagnosticData diagData;
	// End Init ==========================


	// Main BFS loop
	std::vector<std::pair<std::string, int>> result;
	while (!keyQueue.empty())
	{
		if (keyQueue.size() > diagData.MaxQueueSize_OuterBFS) diagData.MaxQueueSize_OuterBFS = keyQueue.size();
		diagData.loopCount_OuterBFS++;
		KeyPosition4D curKeyPos = keyQueue.front(); keyQueue.pop();
		std::vector<std::vector<std::pair<Key, int>>> newKeys = GetAvailableKeyPositionsForSubFields(curKeyPos, field, fieldWidth, visitedKeys, cache, diagData);

		if (!newKeys.size() == 0)
		{
			std::vector<KeyPosition4D> newPositions;
			for (int i = 0; i < 4; i++) // loop the 4 subFields // for (std::vector<std::pair<Key, int>> pVec : newKeys)
			{
				std::vector<std::pair<Key, int>> pVec = newKeys[i];
				for (std::pair<Key, int> p : pVec)
				{
					KeyPosition4D newKeyPosition = curKeyPos;
					newKeyPosition.key[i] = p.first;
					newKeyPosition.steps += p.second;
					std::string newkeys = curKeyPos.keys + p.first.keychar;
					std::sort(newkeys.begin(), newkeys.end());
					newKeyPosition.keys = newkeys;
					newKeyPosition.path += p.first.keychar;
					newPositions.push_back(newKeyPosition);
				}
			}
			for (KeyPosition4D pos : newPositions)
			{
				if (pos.keys.size() == nKeys) // end condition
				{
					//std::cout << "Keys found, route = " << p.path <<", steps = "<<p.steps<<'\n';
					result.push_back({ pos.path,pos.steps });
				}
				else
				{
					//visitedKeys.insert(Hash(p));
					keyQueue.push(pos);
				}
			}
		}
	}
	// Display Results:
	std::cout << "\n\nResults found.\n";
	std::cout << "==============\n";
	std::cout << "Number of keys found: " << nKeys << '\n';
	std::cout << "Number of unique routes: " << result.size() << '\n';

	std::vector<std::pair<std::string, int>>::iterator max_it = std::max_element(result.begin(), result.end(), route_compare);
	std::vector<std::pair<std::string, int>>::iterator min_it = std::min_element(result.begin(), result.end(), route_compare);
	std::cout << "Range of route lengths found: [" << min_it->second << "..." << max_it->second << "]\n";
	std::cout << "Example of shortest path: " << min_it->first << '\n';
	std::cout << "Example of longest path : " << max_it->first << '\n';

	std::cout << "\nAlgorithm diagnostics: \n";
	std::cout << "====================== \n";
	std::cout << "Outer BFS loop (key-to-key): \n";
	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_OuterBFS << '\n';
	std::cout << "> Loop count: " << diagData.loopCount_OuterBFS << '\n';
	std::cout << "Inner BFS loop (cell-to-cell): \n";
	std::cout << "> Maximum queue size: " << diagData.MaxQueueSize_InnerBFS << '\n';
	std::cout << "> Loop count: " << diagData.loopCount_InnerBFS << '\n';
	std::cout << "\nSize of key-to-key cache (#nodes in keyGraph): " << cache.size();

	//std::cout << "\n(press Enter to show keyGraph adjacency list)\n\n";
	//std::cin.get();
	std::cout << "\n\nAdjacency list (Node name is keylocation + keys_owned)\n";
	std::cout << "NODE                                 ARCS\n";
	std::cout << "======================================================\n";
	int count = 0;
	for (auto e : cache)
	{
		std::cout << std::left << std::setw(nKeys + 2) << e.first;
		std::cout << "->  ";
		for (auto p : e.second.cachedAvailablePositions)
		{
			if (p.size() > 0)
			{
				for (int i = 0; i < p.size(); i++)
				{
					std::cout << p[i].first.keychar << "[" << p[i].second << "], ";
				}
			}
		}
		std::cout << '\n';
		if (count == 15) break;
		count++;
	}

	return;
}
void Day19a()
{
	IntCode_simple::IntCode IntComp("Resources/day19input.txt");
	//IntComp.Run();
	int count = 0;
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			count += IntComp.Run(x, y);
			IntComp.Reset();
		}
	}
	std::cout << "Count: " << count;
}
void Day19b_demo()
{
	// We can use IntComputer or load testdata from file.
	bool UseIntComputer = true;
	int boxSize = 3; // size of box that we want to fit
	std::vector<int> field;
	int fieldWidth = 0;
	int fieldHeight = 0;

	if (UseIntComputer == true)
	{
		fieldWidth = 200;
		fieldHeight = (int)((float)fieldWidth * 0.8f);
		std::cout << "Demo with IntComputer (you can set this at start of function), scanning a field of " << fieldWidth << "x" << fieldHeight << " for a box size " << boxSize << " to fit in the beam.\npress Enter.\n";
		std::cin.get();
		IntCode_simple::IntCode IntComp("Resources/day19input.txt");
		// Populate the field
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				int cell = IntComp.Run(x, y);
				IntComp.Reset();
				//if (cell == 1) std::cout << '#'; else std::cout << '.';
				field.push_back(cell);
			}
		}
	}
	else
	{
		std::cout << "Demo with test input from txt file (you can set this at start of function), scanning a field of 40x35 for a box size 10 to fit in the beam.\npress Enter.\n";
		std::cin.get();
		bool fieldWidthSet = false;
		std::ifstream in("Resources/day19testinput.txt");
		while (!in.eof())
		{
			char ch;
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
					std::string str;
					str += ch;
					field.push_back(std::stoi(str));
					i++;
				}
			}
		}
		fieldHeight = field.size() / fieldWidth;
	}
	PrintBeamField(field, fieldWidth);

	std::queue<std::vector<int>> queue;
	for (int y = 0; y < boxSize; y++)
	{
		std::vector<int> line;
		for (int x = 0; x < fieldWidth; x++)
		{
			line.push_back(field[y * fieldWidth + x]);
		}
		queue.push(line);
	}
	int y_SantaSquare = 0;
	while (true)
	{
		int beamwidth_topline = GetBeamWidth(queue.front());
		int firstX_topline = GetFirstBeamXcoordinate(queue.front());
		int beamwidth_bottomline = GetBeamWidth(queue.back());
		int firstX_bottomline = GetFirstBeamXcoordinate(queue.back());
		std::cout << "y coord of topline: " << y_SantaSquare << '\n';
		std::cout << "beamwidth topline line: " << beamwidth_topline << '\n';
		std::cout << "First x coord of topline beam: " << firstX_topline << '\n';
		std::cout << "beamwidth bottom line: " << beamwidth_bottomline << '\n';
		std::cout << "First x coord of bottom line beam: " << firstX_bottomline << '\n';
		PrintQueue(queue);

		int x_SantaSquare = firstX_topline + beamwidth_topline - boxSize;
		if ((beamwidth_topline > boxSize) &&
			(firstX_bottomline == x_SantaSquare))
		{
			std::cout << "Square now fits,  topleft of square: x=" << x_SantaSquare << ", y=" << y_SantaSquare << '\n';
			std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (x_SantaSquare)+(y_SantaSquare)) << '\n'; // lastline correction -1 because of vertical linecount in routine used
			break;
		}
		if (!UseIntComputer) std::cin.get();
		ClearScreen();
		std::vector<int> line;
		for (int x = 0; x < fieldWidth; x++)
		{
			line.push_back(field[(y_SantaSquare + boxSize) * fieldWidth + x]);
		}
		y_SantaSquare++;
		queue.pop();
		queue.push(line);
	}
	return;
}
void Day19b_WithBinarySearch()
{
	// We can use IntComputer or load testdata from file.
	bool UseIntComputer = true;
	int boxSize = 100; // size of box that we need to fit
	int yFirstFit = -1; // these are the coordinates we are looking for
	int xFirstFit = -1;

	if (UseIntComputer)
	{

		IntCode_simple::IntCode IntComp("Resources/day19input.txt");

		// Run a Binary Search loop to establish at what minimum height y the SquareBox fits inside the beam
		// Initialize markers:
		int BS_y_highmark = 0;
		int BS_y_lowmark = boxSize; // begin at minimum possible y-coord to find startpoint
		int BS_y_test = BS_y_lowmark;

		// Find startpoint
		while (BeamWidthAtHeight(BS_y_test, IntComp) < boxSize * 2)
		{
			std::cout << "Finding starting point y = " << BS_y_test << '\n';
			BS_y_test *= 2;
			BS_y_lowmark = BS_y_test;
		}
		std::cout << "-> Found starting point for y: " << BS_y_test << ". Can now start binary search algoritm:\n\n";
		// Run the BS loop	
		while (true)
		{
			std::cout << "Binary search step with y = " << BS_y_test << '\n';
			if (BoxFitsAtHeight(BS_y_test, IntComp, boxSize))
			{
				BS_y_lowmark = BS_y_test;
				BS_y_test = BS_y_highmark + (BS_y_lowmark - BS_y_highmark) / 2;
				if (BS_y_highmark - BS_y_test == 1) // success condition for finding the highest fitting y coordinate (approach from bottom)
				{
					yFirstFit = BS_y_test;
					break;
				}
			}
			else
			{
				if (BS_y_lowmark - BS_y_test == 1) // success condition (approach from top)
				{
					yFirstFit = BS_y_lowmark;
					break;
				}
				BS_y_highmark = BS_y_test;
				BS_y_test = BS_y_test + (BS_y_lowmark - BS_y_test) / 2;
			}
		}
		int beamWidth = BeamWidthAtHeight(yFirstFit, IntComp);
		xFirstFit = GetFirstBeamXcoordinate(GetLine(yFirstFit, IntComp)) + beamWidth - boxSize;

		std::cout << "Topleft of first " << boxSize << "x" << boxSize << " square that fits inside the beam: x=" << xFirstFit << ", y=" << yFirstFit << '\n';
		std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (xFirstFit)+(yFirstFit)) << '\n';
	}
	else
	{
		std::cout << "\n\nExecuting function Day19b_WithBinarySearch().\n";
		std::cout << "Will run a binary search based algorithm on test input from txt file (you can set this at start of function), \nscanning a field of 40x35 for a box size 10 to fit in the beam.\npress Enter.\n";
		std::cin.get();
		boxSize = 10;
		// Load BeamField
		BeamField field;
		{
			std::vector<int> fieldVec_init;
			int fieldWidth_init;
			LoadBeamField(fieldVec_init, fieldWidth_init);
			field.fieldVec = fieldVec_init;
			field.fieldWidth = fieldWidth_init;
			field.fieldHeight = field.fieldVec.size() / field.fieldWidth;
		}
		PrintBeamField(field.fieldVec, field.fieldWidth);


		// Run a Binary Search loop to establish at what minimum height y the SquareBox fits inside the beam
		// Initialize markers:
		int BS_y_highmark = 0;
		int BS_y_lowmark = (field.fieldHeight - 1) - boxSize; // begin at last line of field minus size of box that needs to fit in
		int BS_y_test = BS_y_lowmark;

		// Run the BS loop	
		while (true)
		{
			if (BoxFitsAtHeight(BS_y_test, field, boxSize))
			{
				BS_y_lowmark = BS_y_test;
				BS_y_test = BS_y_highmark + (BS_y_lowmark - BS_y_highmark) / 2;
				if (BS_y_highmark - BS_y_test == 1) // success condition for finding the highest fitting y coordinate (approach from bottom)
				{
					yFirstFit = BS_y_test;
					break;
				}
			}
			else
			{
				if (BS_y_lowmark - BS_y_test == 1) // success condition (approach from top)
				{
					yFirstFit = BS_y_lowmark;
					break;
				}
				BS_y_highmark = BS_y_test;
				BS_y_test = BS_y_test + (BS_y_lowmark - BS_y_test) / 2;
			}
		}
		int beamWidth = BeamWidthAtHeight(yFirstFit, field);
		xFirstFit = GetFirstBeamXcoordinate(GetLine(yFirstFit, field)) + beamWidth - boxSize;

		std::cout << "Topleft of first " << 10 << "x" << 10 << " square that fits inside the beam: x=" << xFirstFit << ", y=" << yFirstFit << '\n';
		std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (xFirstFit)+(yFirstFit)) << '\n';
	}
}

