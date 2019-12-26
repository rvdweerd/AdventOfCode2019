#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <math.h>
#include <windows.h>

struct Pos
{
	int x;
	int y;
	friend bool operator==(Pos& p1, Pos& p2)
	{
		return (p1.x == p2.x) && (p1.y == p2.y);
	}
};
void ReduceToLowestCommonDenom(Pos& direction)
{
	if (direction.x != 0)
	{
		for (int i = max(std::abs(direction.x), std::abs(direction.y)); i > 1; i--)
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
float getClockAngle(std::pair<int, int> q)
{
	float pi = 3.14159265f;
	std::pair<float, float> p = { float(q.first),float(q.second) };
	if (p.first == 0) // on y-axis
	{
		if (p.second >= 0) return 180.0f; else return 0.0f;
	}
	else if (p.second == 0) // on x-axis
	{
		if (p.first >= 0) return 90.0f; else return 270.0f;
	}	
	else if (p.first > 0 && p.second < 0) //1st quadrant
	{
		return (atan(p.first / (-p.second)) * 180 / pi);
	}
	else if (p.first > 0 && p.second > 0) //2nd quadrant
	{
		return (atan((p.second) / p.first) * 180 / pi + 90.0f);
	}
	else if (p.first < 0 && p.second > 0) //3rd quadrant
	{
		return (atan((-p.first) / p.second) * 180 / pi + 180.0f);
	}
	else if (p.first < 0 && p.second < 0) //4th quadrant
	{
		return (atan((p.second) / p.first) * 180 / pi + 270.0f);
	}
	else
	{
		std::cout << "getClockAngle: error in calc of angle using atan\n";
		return 0;
	}
}
enum class Direction
{
	UP = 0,
	RIGHT,
	DOWN,
	LEFT,
	count
};
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
			max_adj = max(max_adj, adj);
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
	int len = (int)input.size();
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
void ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
int nDigits(long long int k)
{
	int n = 0;
	while (k != 0)
	{
		k /= 10;
		n++;
	}
	return n;
}
int LastDigit(int n)
{
	return n % 10;
	//this is stupid:
	//std::string str = std::to_string(n);
	//return (int(str.back() - '0'));
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
struct ChemNode; // forward declaration
struct ProdTables
{
	void Clear()
	{

	}
	std::map<std::string, long long int>& MinProduction;
	//std::map<std::string, long long int>& Stock;
	std::map<std::string, std::vector<R>>& ProductionMap;
	std::map<std::string, ChemNode*>& ChemNodes;
	std::map<std::string, long long int>& OreConvertingChemicals;
};
struct ChemNode
{
	//STILL NEED TO MAKE A DESTRUCTOR FOR THIS DUDE, BUT NO TIME (MEM LEAKS? NOT SURE IF std::vector does garbage collection)
	ChemNode(std::string name, ProdTables& tables)
		:
		name(name)
	{
		for (auto e : tables.ProductionMap[name])
		{
			auto searchnode = tables.ChemNodes.find(e.element);
			if (searchnode != tables.ChemNodes.end())
			{
				//node  already exists, link to existing
				auto supplier = tables.ChemNodes[e.element];
				suppliers.push_back(supplier);
			}
			else
			{
				//create new node
				tables.ChemNodes[e.element] = new ChemNode(e.element, tables);
				ChemNode* newnode = tables.ChemNodes[e.element];
				suppliers.push_back(newnode);
			}
			//if next node = ORE, add to table of base ore converting chemicals
			if (e.element == "ORE") tables.OreConvertingChemicals[name];
		}
	}
	void Purchase(long long int orderSize, ProdTables& tables)
	{
		long long int orderSize_orig = orderSize;
		sold += orderSize;
		// calculate production and manage stock
		if (stock >= orderSize)
		{
			stock -= orderSize;
			return;
		}
		else if (stock > 0)
		{
			orderSize -= stock;
			stock = 0;
		}
		long long int minProduction = tables.MinProduction[name];
		long long int quant = 0;
		if (minProduction >= orderSize) quant = 1; else quant = (long long int)std::ceil(float(orderSize) / float(minProduction));
		produced += quant * minProduction;
		stock = produced - sold;

		// place purchase orders for all elements required for production
		for (auto e : tables.ProductionMap[name])
		{
			tables.ChemNodes[e.element]->Purchase(quant * e.n, tables);
		}
	}
	long long int GetOreConsumption()
	{
		if (name == "ORE") return produced;
		else
		{
			for (auto e : suppliers) return e->GetOreConsumption();
		}
		return 0;
	}
	void Clear()
	{
		produced = 0; sold = 0; stock = 0;
		if (name == "ORE") return;
		else
		{
			for (auto e : suppliers) e->Clear();
			return;
		}
		return;
	}
	std::string name;
	long long int produced = 0;
	long long int sold = 0;
	long long int stock = 0;
	std::vector<ChemNode*> suppliers;
};

