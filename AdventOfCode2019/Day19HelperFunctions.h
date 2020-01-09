#pragma once
#include <vector>
#include <queue>
#include <iostream>
#include "IntCode_simple.h"

void PrintQueue(std::queue<std::vector<int>> queue)
{
	while (!queue.empty())
	{
		std::vector<int> line = queue.front(); queue.pop();
		for (int v : line)
		{
			//std::cout << v;
			if (v == 1) std::cout << 'O'; else std::cout << '.';

		}
		std::cout << '\n';
	}
}
void PrintBeamField(std::vector<int> field, int fieldWidth)
{
	int fieldHeight = (int)field.size() / fieldWidth;
	for (int y = 0; y < fieldHeight; y++)
	{
		for (int x = 0; x < fieldWidth; x++)
		{
			std::cout << field[(size_t)y * fieldWidth + x];
		}
		std::cout << '\n';
	}
}
int GetBeamWidth(std::vector<int> line)
{
	int count = 0;
	for (int v : line)
	{
		count += v;
	}
	return count;
}
int GetFirstBeamXcoordinate(std::vector<int> line)
{
	auto iter = std::find(line.begin(), line.end(), 1);
	return (int)(iter - line.begin());
}
struct BeamField
{
	std::vector<int> fieldVec;
	int fieldWidth;
	int fieldHeight;
};
void LoadBeamField(std::vector<int>& field, int& fieldWidth)
{
	fieldWidth = 1; // we don't know this yet, this is the starting assumption
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
}
int BeamValueAtPosition(int x, int y, IntCode_simple::IntCode& IntComp)
{
	int val = IntComp.Run(x, y);
	IntComp.Reset();
	return val;
}
int BeamValueAtPosition(int x, int y, const BeamField& field)
{
	return field.fieldVec[y * field.fieldWidth + x];
}
std::vector<int> GetLine(int y, IntCode_simple::IntCode& IntComp)
{
	std::vector<int> line;
	for (int x = 0; x < 2 * y; x++)
	{
		line.push_back(IntComp.Run(x, y));
		IntComp.Reset();
	}
	return line;
}
std::vector<int> GetLine(int y, const BeamField& field)
{
	std::vector<int> line;
	for (int x = 0; x < field.fieldWidth; x++)
	{
		line.push_back(field.fieldVec[y * field.fieldWidth + x]);
	}
	return line;
}
int BeamWidthAtHeight(int y, IntCode_simple::IntCode& IntComp)
{
	int count = 0;
	for (int x = 0; x < 2 * y; x++)
	{
		count += IntComp.Run(x, y);
		IntComp.Reset();
	}
	return count;
}
int BeamWidthAtHeight(int y, const BeamField& field)
{
	int count = 0;
	for (int v : GetLine(y, field))
	{
		count += v;
	}
	return count;
}
int IsInBreamRange(int x, int y, IntCode_simple::IntCode IntComp)
{
	return 0;
}
int IsInBreamRange(int x, int y, std::vector<int> field)
{
	return 0;
}
bool BoxFitsAtHeight(int y, IntCode_simple::IntCode& IntComp, int boxSize)
{
	int beamWidth = BeamWidthAtHeight(y, IntComp);
	if (beamWidth >= boxSize)
	{
		int x_BoxTopLeft = GetFirstBeamXcoordinate(GetLine(y, IntComp)) + beamWidth - boxSize;
		if (BeamValueAtPosition(x_BoxTopLeft, y + boxSize - 1, IntComp) == 1)
		{
			return true;
		}
	}
	return false;
}
bool BoxFitsAtHeight(int y, const BeamField& field, int boxSize)
{
	int beamWidth = BeamWidthAtHeight(y, field);
	if (beamWidth >= boxSize)
	{
		int x_BoxTopLeft = GetFirstBeamXcoordinate(GetLine(y, field)) + beamWidth - boxSize;
		if (BeamValueAtPosition(x_BoxTopLeft, y + boxSize - 1, field) == 1)
		{
			return true;
		}
	}
	return false;
}
