#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "IntCode.h"

void GetParameters(int& val, int& A, int& B, int& C, int& DE)
{
	DE = val % 100;
	C = (val / 100) % 10;
	B = (val / 1000) % 10;
	A = (val / 10000) % 10;
}
void Run(std::vector<int>& vec)
{
	int opcode = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	int increment = 0;
	for (int i = 0; vec[i] != 99; i += increment)
	{
		GetParameters(vec[i], A, B, C, opcode);
		if (opcode == 1) // add
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = val1 + val2;
			increment = 4;
		}
		else if (opcode == 2) // multiply
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = val1 * val2;
			increment = 4;
		}
		else if (opcode == 3) // input
		{
			int k;
			std::cout << "Input:"; std::cin >> k;
			vec[vec[i + 1]] = k;
			increment = 2;
		}
		else if (opcode == 4) // output
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			std::cout << val1<<std::endl;
			increment = 2;
		}
		else if (opcode == 4) // output
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			std::cout << val1;
			increment = 2;
		}
		else if (opcode == 5) // jump-if-true
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			if (val1 != 0)
			{
				i = val2;
				increment = 0;
			}
			else
			{
				increment = 3;
			}
		}
		else if (opcode == 6) // jump-if-false
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			if (val1 == 0)
			{
				i = val2;
				increment = 0;
			}
			else
			{
				increment = 3;
			}
		}
		else if (opcode == 7) // less-than
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			
			vec[writePos] = (val1 < val2 ? 1 : 0);
			increment = 4;
		}
		else if (opcode == 8) // equals
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = (val1 == val2 ? 1 : 0);
			increment = 4;
		}
		else
		{
			std::cout << "error.";
		}
	}
}
int RunAutomaticInput(std::vector<int> vec, int in1, int in2)
{
	int opcode = 0;
	int A = 0;
	int B = 0;
	int C = 0;

	int increment = 0;
	for (int i = 0; vec[i] != 99; i += increment)
	{
		GetParameters(vec[i], A, B, C, opcode);
		if (opcode == 1) // add
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = val1 + val2;
			increment = 4;
		}
		else if (opcode == 2) // multiply
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = val1 * val2;
			increment = 4;
		}
		else if (opcode == 3) // input
		{
			int k;
			//std::cout << "Input:"; std::cin >> k;
			k = in1; in1 = in2; //std::cout << "Input entered: " << k << std::endl;
			vec[vec[i + 1]] = k;
			increment = 2;
		}
		else if (opcode == 4) // output
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			//std::cout << val1<<std::endl;
			return val1;
			increment = 2;
		}
		else if (opcode == 4) // output
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			std::cout << val1;
			increment = 2;
		}
		else if (opcode == 5) // jump-if-true
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			if (val1 != 0)
			{
				i = val2;
				increment = 0;
			}
			else
			{
				increment = 3;
			}
		}
		else if (opcode == 6) // jump-if-false
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			if (val1 == 0)
			{
				i = val2;
				increment = 0;
			}
			else
			{
				increment = 3;
			}
		}
		else if (opcode == 7) // less-than
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];

			vec[writePos] = (val1 < val2 ? 1 : 0);
			increment = 4;
		}
		else if (opcode == 8) // equals
		{
			int val1 = (C == 0 ? vec[vec[i + 1]] : vec[i + 1]);
			int val2 = (B == 0 ? vec[vec[i + 2]] : vec[i + 2]);
			int writePos = vec[i + 3];
			vec[writePos] = (val1 == val2 ? 1 : 0);
			increment = 4;
		}
		else
		{
			std::cout << "error.";
		}
	}
	return -1;
}

void printMap(std::vector<std::vector<char>>& map)
{
	for (size_t y = 0; y < map.size(); y++)
	{
		for (size_t x = 0; x < map[0].size(); x++)
		{
			std::cout << map[y][x];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
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
	std::cin.get();
}
void Day2a()
{
	IntCode comp("day2input.txt");
	comp.GetRunCodeVectorReference()[1] = 12;
	comp.GetRunCodeVectorReference()[2] = 2;
	comp.Run();
	std::cout << "Answer to part 1: "<<comp.GetRunCodeVectorReference()[0];
	std::cin.get();
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
	for (int v : comp.GetRunCodeVectorReference())
	{
		std::cout << v << ",";
	}
	std::cout << "Answer to part 2: " << (100 * noun + verb);

	std::cin.get();
}
void Day3()
{
	std::vector<std::vector<char>> map1;
	auto maxx = map1.max_size();
	std::vector<std::vector<char>> map2;
	std::vector<std::vector<char>> mergedMap;
	int fieldSize = 25000;
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
	if (fieldSize < 50)
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
	//printMap(map1);
	//printMap(map2);

	//Traverse map1.
	std::ifstream in2("day3input.txt");
	//in.seekg(0);
	std::map<int, std::pair<int, int>> distdata;
	//distdata[1]; distdata[1].first = 10;
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
	//in.close(); std::ifstream in("Text3.txt");
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
	for (size_t i = 1; i < distdata.size(); i++)
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
bool isValidPassword(int num)
{
	std::string number = std::to_string(num);
	bool flag1 = false;
	int adj = 0; int max_adj = 0;
	std::vector<int> test;
	for (int i = 0; i < 6 - 1; i++)
	{
		if (number[i] == number[i + 1]) 
		{ 
			adj++; 
			max_adj = std::max(max_adj, adj); 
		}
		else 
		{ 
			test.push_back(adj);
			adj = 0; 
		}
		if (number[i + 1] < number[i]) return false;
	}
	test.push_back(adj);
	return ( std::find(test.begin(),test.end(),1) != test.end() );
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
int nToRoot(std::string p, std::map<std::string, std::string>& map)
{
	if (map[p] == "COM")
	{
		return 1;
	}
	else
	{
		return 1 + nToRoot(map[p], map);
	}
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
void permute(std::vector<std::string>& vec, std::string input, std::string sofar)
{
	int len = input.size();
	if (len > 0)
	{
		for (int i = 0; i < len; i++)
		{
			char ch = input[i];
			std::string tmp = input.substr(0, i) + input.substr(i + 1, len);
			permute(vec, tmp, sofar + ch);
		}
	}
	else
	{
		vec.push_back(sofar);
	}
}
void Day7a()
{
	//Load Intcode computers for the Amps
	IntCode ampA("Text7.txt");
	IntCode ampB("Text7.txt");
	IntCode ampC("Text7.txt");
	IntCode ampD("Text7.txt");
	IntCode ampE("Text7.txt");

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
	Day7b();
	

	std::cin.clear();
	std::cin.ignore(5, '\n');
	std::cin.get();
}