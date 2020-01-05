#include "IncludesUsed.h"
#include "PastDaysSolutions.h"
void PrintField(std::vector<char> field, const int& fieldWidth)
{
	//std::cout << '\n';
	for (size_t i = 0; i < field.size(); i++)
	{
		if (i % fieldWidth == 0) std::cout << '\n';
		std::cout << field[i];
	}
	std::cout << "\n";
}
void DebugPrint(std::vector<char> field, const int& fieldWidth, const Pos& pos)
{
	for (char& c : field)
	{
		if (c == '@') c = '.';
	}
	field[pos.y * fieldWidth + pos.x] = '@';
	PrintField(field, fieldWidth);
}
void DebugPrint(std::vector<char> field, const int& fieldWidth, const std::vector<Pos>& startPositions)
{
	for (char& c : field)
	{
		if (c == '@') c = '.';
	}
	for (Pos p : startPositions)
	{
		field[p.y * fieldWidth + p.x] = '@';
	}
	PrintField(field, fieldWidth);
}

void DebugPrintWithActiveQueue(std::vector<char> field, const int& fieldWidth, const Pos& pos, std::queue<Pos> queue)
{
	for (char& c : field)
	{
		if (c == '@') c = '.';
	}
	field[pos.y * fieldWidth + pos.x] = '@';
	while (!queue.empty())
	{
		Pos p = queue.front(); queue.pop();
		field[p.y * fieldWidth + p.x] = '$';
	}
	PrintField(field, fieldWidth);
}

void DebugPrintWithActiveQueue(std::vector<char> field, const int& fieldWidth, const std::vector<Pos>& pos, std::queue<std::vector<Pos>> queue)
{
	for (char& c : field)
	{
		if (c == '@') c = '.';
	}
	for (Pos p : pos)
	{
		field[p.y * fieldWidth + p.x] = '@';
	}
	while (!queue.empty())
	{
		std::vector<Pos> v = queue.front(); queue.pop();
		for (Pos p : v)
		{
			field[p.y * fieldWidth + p.x] = '$';
		}
	}
	PrintField(field, fieldWidth);
}


std::string SerializeIntoString(Pos pos) // used to enable a hash lookup of visited nodes in the BFS
{
	std::sort(pos.keys.begin(), pos.keys.end());
	pos.keys.append(std::to_string(pos.x));
	pos.keys.append("_");
	pos.keys.append(std::to_string(pos.y));
	return pos.keys;
}
std::vector<Pos> FindNeighborsNoHovering(const std::vector<char>& field, const int& fieldWidth, const Pos& pos, const std::set<std::string>& visited)
{
	std::vector<Pos> availablePositions;
	int newX, newY;
	for (int dir = 1; dir < 5; dir++)
	{
		if (dir == 1) // NORTH
		{
			newX = pos.x;
			newY = pos.y - 1;
		}
		else if (dir == 2) // SOUTH
		{
			newX = pos.x;
			newY = pos.y + 1;
		}
		else if (dir == 3) // WEST
		{
			newX = pos.x -1;
			newY = pos.y;
		}
		else if (dir == 4) // EAST
		{
			newX = pos.x + 1;
			newY = pos.y;
		}
		
		char ch = field[newY * fieldWidth + newX];
		if ((ch == '#') || // If neighbor is [wall] or [door without key] don't include
			(ch > 64 && ch <= 90 && (pos.keys.find(std::tolower(ch)) == std::string::npos)) )
		{
		}
		else // Else, include neighbor, if not already visited, add key if found
		{
			std::string newkeys = pos.keys;
			if (ch > 96 && ch <= 122 && (newkeys.find(ch) == std::string::npos)) // If new key is found, add it to newpos.keys
			{
				newkeys += ch; // This will retain the order in which the keys are found. We sort when serializing
			}
			Pos newpos = { newX,newY,pos.n+1, newkeys };
			if (visited.find(SerializeIntoString(newpos)) == visited.end())
			{
				availablePositions.push_back(newpos);
			}
		}
	}
	return availablePositions;
}
std::vector<Pos> LoadField(std::string filename, std::vector<char>& field, int& fieldWidth, int& nKeys)
{
	fieldWidth = 1; // we don't know this yet, this is the starting assumption
	bool fieldWidthSet = false;
	std::ifstream in(filename);
	int countStartPositions = 0;
	std::vector<Pos> startPositions;
	while (!in.eof())
	{
		char ch;
		int i = 0;
		std::string str;
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
				if (ch == '@')
				{
					Pos startpos;
					startpos.y = i / fieldWidth;
					startpos.x = i - startpos.y * fieldWidth;
					//startpos.quarter = std::string("Q") + std::to_string(countStartPositions + 1);
					startpos.quarter = "Q" + std::to_string(countStartPositions + 1);
					countStartPositions++;
					startPositions.push_back(startpos);
				}
				field.push_back(ch);
				i++;
			}
		}
	}

	// Scan for number of keys in the maze
	nKeys = 0; // in case not initialized by caller
	for (char c : field)
	{
		if (c >= 97 && c < 123)
		{
			nKeys++;
		}
	}
	return startPositions;
}
void Day18a()
{
	bool DEBUG = true;
	// Initialize
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::vector<Pos> startPositions = LoadField("Resources/day18input.txt", field, fieldWidth, nKeys);
	//std::vector<Pos> startPositions = LoadField("Resources/day18binput.txt", field, fieldWidth, nKeys);
	if (startPositions.size() != 1) std::cout << "Error loading field.";
	Pos startpos = startPositions[0];
	
	if (DEBUG)
	{
		std::cout << "STARTING GRID\n";
		DebugPrint(field, fieldWidth, startpos);
		std::cout << "\nMOVEMENT TRACKING\n";
	}
	
	// BFS to find all keys
	std::queue<Pos> queue;
	std::set<std::string> visited;
	queue.push(startpos);
	visited.insert(SerializeIntoString(startpos));
	

	while (!queue.empty())
	{
		Pos currentpos = queue.front(); queue.pop();
		std::vector<Pos> neighbors = FindNeighborsNoHovering(field, fieldWidth, currentpos, visited);
		
		if (DEBUG)
		{
			std::cout << "\nMain loop. \n==========\nFront queue position taken. ";
			std::cout << "New queue size: " << queue.size() << '\n';
			std::cout << "Current position investigated: (" << currentpos.x << "," << currentpos.y << "). Cumulative steps: " << currentpos.n << ".\n";
			if (neighbors.size() > 0)
			{
				std::cout << "Found " << neighbors.size() << " accessible neighbors: ";
				for (Pos p : neighbors)
				{
					std::cout << "(" << p.x << ", " << p.y << "); ";
				}
				std::cout << neighbors.size() << " positions added to the queue.\n";
			}
			else
			{
				std::cout << "Found no accessible neighbors. ";
				std::cout<<"Nothing added to the queue.\n";
			}

			std::cout << "Current key string: " << currentpos.keys << '\n';
			DebugPrintWithActiveQueue(field, fieldWidth, currentpos, queue);
		}
		
		for (Pos p : neighbors)
		{
			if (p.keys.size() == nKeys)
			{
				std::cout << "Last step to: (" << p.x << "," << p.y << "), keys = " << p.keys << std::endl;
				std::cout << "All keys obtained, " << p.n << " steps taken.";
				while (!queue.empty()) queue.pop();
				break;
			}
			visited.insert(SerializeIntoString(p));
			queue.push(p);
			
			if (DEBUG)
			{
				std::cout << "Adding serialized position to the visited set: " << SerializeIntoString(p);
				std::cout << "; New queue size: " << queue.size() << '\n';
			}
		}
		std::cin.get();
	}
}
std::string SerializeIntoString(std::vector<Pos> startPositions) // used to enable a hash lookup of visited nodes in the BFS
{
	std::string outputString;
	for (Pos p : startPositions)
	{
		std::sort(p.keys.begin(), p.keys.end());
		outputString += p.keys;
		outputString.append(std::to_string(p.x));
		outputString.append("_");
		outputString.append(std::to_string(p.y));
		outputString.append(";");
	}
	return outputString;
}
std::vector<std::vector<Pos>> FindNeighborsNoHovering(	const std::vector<char>& field, 
														const int& fieldWidth, 
														const std::vector<Pos>& currentPositions, 
														const std::set<std::string>& visited,
														std::string& globalKeys )
{
	std::vector<std::vector<Pos>> availablePositionVectors;
	for (size_t i = 0; i < currentPositions.size(); i++)
	{
		Pos pos = currentPositions[i];
		//std::vector<Pos> availablePositions;
		int newX, newY;
		for (int dir = 1; dir < 5; dir++)
		{
			if (dir == 1) // NORTH
			{
				newX = pos.x;
				newY = pos.y - 1;
			}
			else if (dir == 2) // SOUTH
			{
				newX = pos.x;
				newY = pos.y + 1;
			}
			else if (dir == 3) // WEST
			{
				newX = pos.x - 1;
				newY = pos.y;
			}
			else if (dir == 4) // EAST
			{
				newX = pos.x + 1;
				newY = pos.y;
			}

			char ch = field[newY * fieldWidth + newX];
			if ((ch == '#') || // If neighbor is [wall] or [door without key] don't include
				(ch > 64 && ch <= 90 && (globalKeys.find(std::tolower(ch)) == std::string::npos)))
			{
			}
			else // Else, include neighbor, if not already visited, add key if found
			{
				std::vector<Pos> newPositionsVec = currentPositions;

				std::string newkeys = pos.keys;
				if (ch > 96 && ch <= 122 && (globalKeys.find(ch) == std::string::npos)) // If new global key is found, it's to your credit and your keyring is updated to global
				{
					globalKeys += ch;
					newkeys += ch;
				}
				else if (ch > 96 && ch <= 122 && (pos.keys.find(ch) == std::string::npos)) // You encounter a previously found key
				{
					newkeys += ch;
				}
				Pos newpos = { newX,newY,pos.n + 1, newkeys ,pos.quarter};
				newPositionsVec[i] = newpos;
				if (visited.find(SerializeIntoString(newPositionsVec)) == visited.end())
				{
					availablePositionVectors.push_back(newPositionsVec);
				}
			}
		}
	}
	return availablePositionVectors;
}
void Day18b()
{
	bool DEBUG = true;
	// Initialize
	std::vector<char> field;
	int fieldWidth;
	int nKeys;
	std::vector<Pos> startPositions = LoadField("Resources/day18binput.txt", field, fieldWidth, nKeys);
	std::string globalKeys;
	
	if (DEBUG)
	{
		std::cout << "STARTING GRID\n";
		DebugPrint(field, fieldWidth, startPositions);
		std::cout << "\nMOVEMENT TRACKING\n";
	}
	   
	// BFS to find all keys
	std::queue<std::vector<Pos>> queue;
	std::set<std::string> visited;
	queue.push(startPositions);
	visited.insert(SerializeIntoString(startPositions));


	while (!queue.empty())
	{
		std::vector<Pos> currentPositions = queue.front(); 
		queue.pop();
		std::vector<std::vector<Pos>> neighborVecs = FindNeighborsNoHovering(field, fieldWidth, currentPositions, visited, globalKeys);

		if (DEBUG)
		{
			ClearScreen();
			DebugPrint(field, fieldWidth, currentPositions);
			std::cout << "\nMain loop. \n==========\nFront queue position taken. ";
			std::cout << "New queue size: " << queue.size() << '\n';
			std::cout << "Current positions investigated:\n";
			for (Pos p : currentPositions)
			{
				std::cout<< "(" << p.x << ", " << p.y << "),n=" << p.n << ", "<<p.quarter<<", pos.keys="<<p.keys<<".\n";
			}
				
			if (neighborVecs.size() > 0)
			{
				std::cout << "Found " << neighborVecs.size() << " accessible neighbor vectors:\n";
				for (std::vector<Pos> v : neighborVecs)
				{
					for (Pos p : v)
					{
						std::cout << "(" << p.x << ", " << p.y << "),n=" << p.n << ", " << p.quarter << ", pos.keys=" << p.keys << ".\n";
					}
					std::cout << "------------------------------";
				}
				std::cout << neighborVecs.size() << " new position vectors added to the queue.\n";
			}
			else
			{
				std::cout << "Found no accessible neighbors. ";
				std::cout << "Nothing added to the queue.\n";
			}

			std::cout << "Current global key string: " << globalKeys << '\n';
			DebugPrintWithActiveQueue(field, fieldWidth, currentPositions, queue);
		}
			   		 
		for (std::vector<Pos> pVec : neighborVecs)
		{
			
			if (globalKeys.size() == nKeys)
			{
				int result = 0;
				std::string check;
				for (Pos p : currentPositions)
				{
					result += p.n;
					check += p.keys;
				}
				//std::cout << "Last step to: (" << p.x << "," << p.y << "), keys = " << p.keys << std::endl;
				if (check.size() == nKeys)
				{
					std::cout << "All keys obtained, " << result << "steps taken.";
					while (!queue.empty()) queue.pop();
					break;
				}
			}
			visited.insert(SerializeIntoString(pVec));
			queue.push(pVec);
		}
		if (DEBUG)
		{
			std::cin.get();
		}
		
	}

}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day18a();
	Day18b();
	
	while (!_kbhit());
	return 0;
}

