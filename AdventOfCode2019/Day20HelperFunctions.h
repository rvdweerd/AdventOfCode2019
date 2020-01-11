#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#include <map>
#include <conio.h>
#include <queue>

namespace MazeDay20
{
	class Maze
	{
	public: // Interface
		Maze(bool recOption)
			:
			isRecursiveMaze(recOption)
		{
			LoadField();
			MapPortals();
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
			startpos.history.push_back({ startpos.x,startpos.y,startpos.recursiveDepthLevel });
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
					if (visited.find(HashD20(n)) == visited.end())
					{
						visited.insert(HashD20(n));
						queue.push(n);
					}
					if (n.x == endpos.x && n.y == endpos.y && n.recursiveDepthLevel == 0)
					{
						nSteps = n.steps;
						PrintFieldWithPathway(n.history);
						while (!queue.empty()) queue.pop();
						break;
					}
				}
			}
			return nSteps;
		}

	private: // Support methods
		void PrintField()
		{
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					std::cout << GetFieldChar(x, y);
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
		void PrintFieldWithVisited(std::set<std::string>& visited)
		{
			std::vector<char> fieldCopy = this->field;

			std::stringstream coord;
			for (std::string str : visited)
			{
				Coi2 p;
				std::stringstream cstream(str);
				cstream >> p.x;
				cstream.get();
				cstream >> p.y;
				fieldCopy[p.y * fieldWidth + p.x] = 'o';
			}
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					char ch = fieldCopy[y * fieldWidth + x];
					if (ch == '.') std::cout << ' ';
					else std::cout << fieldCopy[y * fieldWidth + x];
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
		void PrintFieldWithPathway(std::vector<mazeHistory> history)
		{
			std::vector<char> fieldCopy = this->field;

			for (mazeHistory h : history)
			{
				fieldCopy[h.y * fieldWidth + h.x] = std::to_string(h.recDepth)[0];
			}
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					char ch = fieldCopy[y * fieldWidth + x];
					if (ch == '.') std::cout << ' ';
					else std::cout << fieldCopy[y * fieldWidth + x];
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
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
			std::string warpStartString;
			warpStartString += ch1; warpStartString += ch2; std::sort(warpStartString.begin(), warpStartString.end());
			if (portals.find(warpStartString) != portals.end())
			{
				std::vector<Coi2> warpPositions = portals[warpStartString];
				for (Coi2& p : warpPositions) // copy all attributes associated with current position under evaluation
				{
					Coi2 p_tmp = pos;
					p_tmp.history.push_back({ p.x,p.y,p.recursiveDepthLevel });
					p.history = p_tmp.history;
					p.recursiveDepthLevel = p_tmp.recursiveDepthLevel;
					p.steps = p_tmp.steps+1; // neighbor position implies 1 step added
				}
				if (warpPositions.size() == 1) // We are at start (AA) or end (ZZ) of maze, as these only have one instance
				{
					// When the maze is recursive (1) we only add the associated position when we are at recursive level 0 (2)
					if (!isRecursiveMaze || pos.recursiveDepthLevel==0)
					{
						//warpPositions[0].steps = pos.steps;
						outputVec.push_back(warpPositions[0]);
					}
				}
				else
				{
					if (isRecursiveMaze)
					{
						if (warpPositions[0] == pos) // Our current location is on inner donut, we are warping INward (deeper, increasing recursive depth)
						{
							warpPositions[1].recursiveDepthLevel++;
							if (warpPositions[1].recursiveDepthLevel < portals.size())  // we don't need to explore more depth than #warp portals
							outputVec.push_back(warpPositions[1]);
						}
						else // Our current location is on outer donut, we are warping OUTward (shallowe, decreasing recursive depth)
						{
							if (pos.recursiveDepthLevel != 0) // we can only warp OUTward if are not on recursive depth 0
							{
								warpPositions[0].recursiveDepthLevel--;
								outputVec.push_back(warpPositions[0]);
							}
						}
					}
					else
					{
						if (warpPositions[0] == pos) outputVec.push_back(warpPositions[1]);
						else outputVec.push_back(warpPositions[0]);
					}
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
								Coi2 p_tmp = pos;
								p_tmp.history.push_back({ pos.x,pos.y - 1,pos.recursiveDepthLevel });
								outputVec.push_back({ pos.x, pos.y - 1,pos.steps+1,p_tmp.history,pos.recursiveDepthLevel });
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
								Coi2 p_tmp = pos;
								p_tmp.history.push_back({ pos.x,pos.y + 1,pos.recursiveDepthLevel });
								outputVec.push_back({ pos.x, pos.y + 1,pos.steps+1,p_tmp.history,pos.recursiveDepthLevel });
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
								Coi2 p_tmp = pos;
								p_tmp.history.push_back({ pos.x-1,pos.y ,pos.recursiveDepthLevel });
								outputVec.push_back({ pos.x-1, pos.y ,pos.steps+1,p_tmp.history,pos.recursiveDepthLevel });
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
								Coi2 p_tmp = pos;
								p_tmp.history.push_back({ pos.x+1,pos.y ,pos.recursiveDepthLevel });
								outputVec.push_back({ pos.x+1, pos.y,pos.steps+1,p_tmp.history,pos.recursiveDepthLevel });
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
			str.append("_");
str.append(std::to_string(pos.recursiveDepthLevel));
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
			mazeCenterPoint = { ((int)fieldWidth - 1) / 2,((int)fieldHeight - 1) / 2 };
		}
		void MapPortals()
		{
			for (size_t y = 0; y < fieldHeight; y++)
			{
				for (size_t x = 0; x < fieldWidth; x++)
				{
					Coi2 pos = { x,y };
					char ch1 = GetFieldChar(x, y);
					if (IsCap(ch1))
					{
						int dir = NeighborIsDot(x, y);
						if (dir != 0)
						{
							char ch2 = GetFieldChar(NextPosInDir(pos, OpposideDir(dir)));
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
			//	Make sure all warp destination vectors are sorted based on distance from centerpoint of the maze. 
			//	Reason: accounting for maze recursive depth (puzzle part 2)
			for (auto& e : portals)
			{
				// This is a very interesting and crucial aspect of Day20-part 2: we determine whether warp stations are on the inner or outer
				// side of the donut. The way we do this is by making sure the map values are always sorted from <inner donut> to <outer donut>
				// positions.

				// My first thought was sorting by distance from the centerposition of the maze. Unpon deeper reflection though,
				// this is not correct. We need discrete logic to determine the sorting order.

				// SO THIS DID NOT WORK:
				// We sort using a lambda function with capture clause [] to reference data member mazeCenterPos to determine the sorting order
				// Coi2 center = mazeCenterPoint;
				/*
				std::sort(e.second.begin(), e.second.end(), [this](Coi2 a, Coi2 b)
					{
						int a_distanceFromCenter = (abs(a.x - mazeCenterPoint.x) + abs(a.y - mazeCenterPoint.y));
						int b_distanceFromCenter = (abs(b.x - mazeCenterPoint.x) + abs(b.y - mazeCenterPoint.y));
						return (a_distanceFromCenter < b_distanceFromCenter);
					});
				*/

				// BUT THIS DOES:
				if (e.second[0].x == 2 || e.second[0].x == (fieldWidth - 3) ||  // this means this warp position is on one of the outer donut verticals
					e.second[0].y == 2 || e.second[0].y == (fieldHeight - 3) )  // this  ,,    ,,   ,,      ,,    ,,   ,,    ,,     ,,    ,,  horizontals
				{
					std::reverse(e.second.begin(), e.second.end());
				}
			}
		}

	private: // Data members
		std::map<std::string,std::vector<Coi2>> portals;
		std::vector<char> field;
		size_t fieldWidth;
		size_t fieldHeight;
		Coi2 mazeCenterPoint;
		const bool isRecursiveMaze;
	};
}
