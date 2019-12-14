#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <conio.h>
#include "IntCode.h"
#include "HelperFunctions.h"

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
				additional = std::max(0, additional / 3 - 2);
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
	for (int v : comp.GetRunCodeVectorReference())
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
	for (size_t i = 1; i <= distdata.size(); i++)
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
				minSteps = std::min(minSteps, p.second + q.second);
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
	int maxResult = -1;
	std::string maxOption;
	for (auto str : options)
	{
		int resA = ampA.Run(str[0] - '0', 0)		; ampA.Reset();
		int resB = ampB.Run(str[1] - '0', resA)		; ampB.Reset();
		int resC = ampC.Run(str[2] - '0', resB)		; ampC.Reset();
		int resD = ampD.Run(str[3] - '0', resC)		; ampD.Reset();
		int tmpResult = ampE.Run(str[4] - '0', resD); ampE.Reset();
		
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
	int maxResult = -1;
	std::string maxOption;

	for (auto str : options)
	{
		int tmpResult = 0;
		// First run to set the phases
		int resA = ampA.Run(str[0] - '0',0);
		int resB = ampB.Run(str[1] - '0',resA);
		int resC = ampC.Run(str[2] - '0',resB);
		int resD = ampD.Run(str[3] - '0',resC);
		int resE = ampE.Run(str[4] - '0',resD);

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

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day7a();
	
	while (!_kbhit());
}