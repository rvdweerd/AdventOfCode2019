#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

struct Pos
{
	int x;
	int y;
	//friend Pos operator-(Pos& p1, Pos& p2);
};
void ReduceToLowestCommonDenom(Pos& direction)
{
	if (direction.x != 0)
	{
		for (int i = std::max(std::abs(direction.x), std::abs(direction.y)); i > 1; i--)
		{
			if (direction.y % i == 0 && direction.x % i == 0)
			{
				direction.y /= i;
				direction.x /= i;
			}
		}
	}
	if (direction.x == 0 && direction.y != 0)
	{
		direction.y /= std::abs(direction.y);
	}
}
/*
Pos operator-(Pos& p1, Pos& p2)
{
	return { p1.x - p2.x ,p1.y - p2.y };
}
*/

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
	return (std::find(test.begin(), test.end(), 1) != test.end());
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
