#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <conio.h>
#include <iomanip>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include <thread>
#include "PlanetSys.h"
#include <utility>
#include <stack>
#include "Maze.h"
#include "Maze_simple.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>

namespace MazeDay20
{
	class Maze
	{
	public:
		Maze()
		{
			LoadField();
			MapPortals();
		}
		void PrintField()
		{
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					std::cout << GetFieldChar(x,y);
				}
				std::cout << '\n';
			}
		}
		void PrintFieldWithVisited(std::set<std::string>& visited)
		{
			//std::vector<char> fieldCopy = this->field;
			//for (Coi2 p : visited)
			//{
			//	fieldCopy[p.y * fieldWidth + p.x] = 'X';
			//}
			//for (size_t y = 0; y < fieldHeight; y++)
			//{
			//	for (size_t x = 0; x < fieldWidth; x++)
			//	{
			//		std::cout << fieldCopy[y*fieldWidth+x];
			//	}
			//	std::cout << '\n';
			//}
		}
		Coi2 GetStartPos()
		{
			return portals["AA"][0];
		}
		Coi2 GetEndPos()
		{
			return portals["ZZ"][0];
		}
		int ShortestPath(Coi2 startpos, Coi2 endpos)
		{
			std::queue<Coi2> queue;
			queue.push(startpos);
			std::set<std::string> visited;
			visited.insert(HashD20(startpos));
			int nSteps = 0;

			while (!queue.empty())
			{
				//ClearScreen();
				//PrintFieldWithVisited(visited);
				//std::cin.get();
				Coi2 currentpos = queue.front(); queue.pop();
				std::vector<Coi2> neighbors = GetNeighborsD20(currentpos);
				for (Coi2 n : neighbors)
				{
					if (n.x == endpos.x && n.y == endpos.y)
					{
						nSteps = n.steps;
						break;
					}
					if (visited.find(HashD20(n)) == visited.end())
					{
						n.steps++;
						visited.insert(HashD20(n));
						queue.push(n);
					}
				}
			}
			return nSteps+1;
		}

	private: // Support methods
		char GetFieldChar(int x, int y)
		{
			return field[y * fieldWidth + x];
		}
		char GetFieldChar(Coi2 pos)
		{
			return field[pos.y * fieldWidth + pos.x];
		}
		void GetWarpPosition(Coi2 pos, char ch1, int dir, std::vector<Coi2>& outputVec)
		{
			char ch2 = GetFieldChar(NextPosInDir(NextPosInDir(pos, dir), dir));
			std::string str;
			str += ch1; str += ch2; std::sort(str.begin(), str.end());
			if (portals.find(str) != portals.end())
			{
				std::vector<Coi2> warpPositions = portals[str];
				if (warpPositions.size() == 1)
				{
					warpPositions[0].steps = pos.steps;
					outputVec.push_back(warpPositions[0]);
				}
				else
				{
					warpPositions[0].steps = pos.steps;
					warpPositions[1].steps = pos.steps;
					if (warpPositions[0] == pos) outputVec.push_back(warpPositions[1]);
					else outputVec.push_back(warpPositions[0]);
				}
			}
		}
		std::vector<Coi2> GetNeighborsD20(Coi2 pos)
		{
			std::vector<Coi2> outputVec;
			for (int dir = 1; dir < 5; dir++)
			{
				switch (dir)
				{
				case 1:
					if (pos.y != 0 || pos.y != fieldHeight - 1)
					{
						char ch = GetFieldChar(pos.x,pos.y-1);
						if (ch != '#')
						{
							if (ch == '.')
							{
								outputVec.push_back({ pos.x, pos.y - 1,pos.steps });
							}
							else
							{
								GetWarpPosition(pos, ch, dir, outputVec);
							}
						}
					}
					break;
				case 2:
					if (pos.y != 0 || pos.y != fieldHeight - 1)
					{
						char ch = GetFieldChar(pos.x, pos.y + 1);
						if (ch != '#')
						{
							if (ch == '.')
							{
								outputVec.push_back({ pos.x, pos.y + 1,pos.steps });
							}
							else
							{
								GetWarpPosition(pos, ch, dir, outputVec);
							}
						}
					}
					break;
				case 3:
					if (pos.x != 0 || pos.x != fieldWidth - 1)
					{
						char ch = GetFieldChar(pos.x-1, pos.y);
						if (ch != '#')
						{
							if (ch == '.')
							{
								outputVec.push_back({ pos.x-1, pos.y ,pos.steps });
							}
							else
							{
								GetWarpPosition(pos, ch, dir, outputVec);
							}
						}
					}
					break;
				case 4:
					if (pos.x != 0 || pos.x != fieldWidth - 1)
					{
						char ch = GetFieldChar(pos.x+1, pos.y);
						if (ch != '#')
						{
							if (ch == '.')
							{
								outputVec.push_back({ pos.x+1, pos.y,pos.steps });
							}
							else
							{
								GetWarpPosition(pos, ch, dir, outputVec);
							}
						}
					}
					break;
				}
			}
			return outputVec;
		}
		int NeighborIsDot(int x, int y)
		{
			for (int dir = 1; dir < 5; dir++)
			{
				switch (dir)
				{
				case 1: 
					if (y == 0 || y == fieldHeight - 1) return 0;
					if (GetFieldChar(x, y - 1) == '.') return dir;
					break;
				case 2:
					if (y == 0 || y == fieldHeight - 1) return 0;
					if (GetFieldChar(x, y + 1) == '.') return dir;
					break;
				case 3:
					if (x == 0 || x == fieldWidth - 1) return 0;
					if (GetFieldChar(x-1, y) == '.') return dir;
					break;
				case 4:
					if (x == 0 || x == fieldWidth - 1) return 0;
					if (GetFieldChar(x + 1, y) == '.') return dir;
					break;
				}
			}
			return 0;
		}
		int NeighborIsCap(int x, int y)
		{
			for (int dir = 1; dir < 5; dir++)
			{
				switch (dir)
				{
				case 1:
					if (y == 0 || y == fieldHeight - 1) return 0;
					if (IsCap(GetFieldChar(x, y - 1))) return dir;
					break;
				case 2:
					if (y == 0 || y == fieldHeight - 1) return 0;
					if (IsCap(GetFieldChar(x, y + 1))) return dir;
					break;
				case 3:
					if (x == 0 || x == fieldWidth - 1) return 0;
					if (IsCap(GetFieldChar(x-1, y))) return dir;
					break;
				case 4:
					if (x == 0 || x == fieldWidth - 1) return 0;
					if (IsCap(GetFieldChar(x+1, y))) return dir;
					break;
				}
			}
			return 0;
		}
		Coi2 NextPosInDir(Coi2 pos, int dir)
		{
			switch (dir)
			{
			case 1:
				assert(pos.y > 0);
				return { pos.x,pos.y - 1 };
				break;
			case 2:
				assert(pos.y < fieldHeight - 1);
				return { pos.x,pos.y + 1 };
				break;
			case 3:
				assert(pos.x > 0);
				return { pos.x - 1,pos.y };
				break;
			case 4:
				assert(pos.x < fieldWidth - 1);
				return { pos.x + 1,pos.y };
				break;
			}
			return { -1,-1 };
		}
		std::string HashD20(Coi2 pos)
		{
			std::string str;
			str.append(std::to_string(pos.x));
			str.append("_");
			str.append(std::to_string(pos.y));
			return str;
		}

	private: // Initialization methods
		void LoadField()
		{
			fieldWidth = 1; // we don't know this yet, this is the starting assumption
			bool fieldWidthSet = false;
			std::ifstream in("day20input.txt");
			while (!in.eof())
			{
				char ch;
				std::string str;
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
						field.push_back(ch);
						i++;
					}
				}
			}
			fieldHeight = field.size() / fieldWidth;
		}
		void MapPortals()
		{
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					Coi2 pos = { x,y };
					char ch1 = GetFieldChar(x,y);
					if (IsCap(ch1))
					{
						int dir = NeighborIsDot(x, y);
						if (dir != 0)
						{
							char ch2 = GetFieldChar(NextPosInDir(pos,OpposideDir(dir)));
							std::string str; str += ch2; str += ch1; std::sort(str.begin(), str.end());
							Coi2 nextpos = NextPosInDir(pos, dir);
							if (std::find(portals[str].begin(), portals[str].end(), nextpos) == portals[str].end())
							{
								portals[str].push_back(nextpos);
							}
						}
						else
						{
							dir = NeighborIsCap(x, y);
							if (dir != 0)
							{
								char ch2 = GetFieldChar(NextPosInDir(pos, dir));
								std::string str; str += ch1; str += ch2; std::sort(str.begin(), str.end());
								Coi2 nextpos = NextPosInDir(NextPosInDir(pos, dir), dir);
								if (std::find(portals[str].begin(), portals[str].end(), nextpos) == portals[str].end())
								{
									portals[str].push_back(nextpos);
								}
							}

						}
					}
				}
			}
		}

	private: // Data members
		std::map<std::string,std::vector<Coi2>> portals;
		std::vector<char> field;
		size_t fieldWidth;
		size_t fieldHeight;
	};
}
