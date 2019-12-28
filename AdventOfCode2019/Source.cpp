#include "IncludesUsed.h"
#include "PastDaysSolutions.h"

class Field
{
public:
	Field()
		:
		ASCIIcomp("Resources/day17input.txt")
	{
		{
			int xcounter = 0;
			int ycounter = 0;
			char ch_prev = 0;
			std::cout << "***** START OF INITIALIZATION\n";
			for (char ch = ASCIIcomp.Run(0); ch != 99; ch = ASCIIcomp.Run(0))
			{
				if (ch == 10 && ch_prev == 10) break;
				if (ch == 10) {
					ycounter++; width = xcounter; xcounter = 0;
				}
				else {
					std::pair<int, int> pos(xcounter, ycounter);
					field.push_back(ch);
					xcounter++;
					if (ch == '#')
					{
						scafPositions.insert(pos);
					}
					else if (ch != '.')
					{
						robotPos = { xcounter,ycounter };
						scafPositions.insert(robotPos);
						switch (ch)
						{
						case '<':
							robotDir = 3;
							break;
						case '>':
							robotDir = 4;
							break;
						case '^':
							robotDir = 1;
							break;
						case 'v':
							robotDir = 2;
							break;
							std::cout << "Error, character in field not recognized";
						}
					}
				}
				std::cout << ch;
				ch_prev = ch;
			}
			height = ycounter;
		}
		int calibration_at_start = 0;
		for (auto pos : scafPositions)
		{
			auto e = scafPositions.end();
			int neighbors = 0;
			if (scafPositions.find({ pos.first + 1,pos.second     }) != e) neighbors++;
			if (scafPositions.find({ pos.first - 1,pos.second     }) != e) neighbors++;
			if (scafPositions.find({ pos.first    ,pos.second + 1 }) != e) neighbors++;
			if (scafPositions.find({ pos.first    ,pos.second - 1 }) != e) neighbors++;
			if (neighbors>2)
			{ 
				scafIntersections.insert(pos);
				calibration_at_start += pos.first * pos.second;
			}
		}
		std::cout << "\nField dimensions: w=" << width << ", h=" << height << std::endl;
		std::cout << "Position of Robot: x=" << robotPos.first << ", y=" << robotPos.second << std::endl;
		std::cout << "Direction code for Robot (1=UP, DOWN, WEST, EAST): " << robotDir << std::endl;
		std::cout << "Number of scaffold positions: " << scafPositions.size() << std::endl;
		std::cout << "Number of intersections: " << scafIntersections.size() << std::endl;
		std::cout << "Intersections are at: ";
		for (auto pos : scafIntersections) std::cout << "(" << pos.first << "," << pos.second << "),"; std::cout << std::endl;
		std::cout << "Calibration value: " << calibration_at_start << std::endl;
		std::cout << "***** END OF INITIALIZATION\n";
		//ASCIIcomp.Reset();
	}
	void Print()
	{
		
	}
private:
	IntCode ASCIIcomp;
	std::vector<char> field;
	int width=0;
	int height=0;
	std::set<std::pair<int,int>> scafPositions;
	std::set<std::pair<int, int>> scafIntersections;
	std::pair<int, int> robotPos = { 0,0 };
	int robotDir = 0;

};

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
	std::vector<char> Main  = { 'A',44,'B',44,'A',44,'C',44,'A',44,'B',44,'C',44,'B',44,'C',44,'A', 10 };
	std::vector<char> FuncA = { 'L',44,6+'0',44,6 + '0',44,'R',44,4 + '0',44,'R',44,4 + '0',44,'L',44,6 + '0',		10 };
	std::vector<char> FuncB = { 'L',44,6 + '0',44,6 + '0',44,'R',44,4 + '0',44,'R',44,4 + '0',44,'R',44,6 + '0',44,6+'0',	10 };
	std::vector<char> FuncC = { 'L',44,5 + '0',44,5 + '0',44,'L',44,6 + '0',44,'R',44,4 + '0', 10};
	std::vector<char> Other = { 'n',							10};
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

void Infi1()
{	//aoc.infi.nl
	// Test input
	std::vector<std::pair<int, int>> flats = { {1,4},{3,8},{4,3},{5,7},{7,4},{10,3} };
	std::vector < std::pair<int, int>> sprongen = { {2,0},{0,4},{1,0 },{0,0} };

	// Opdracht input
	//std::vector<std::pair<int, int>> flats = { { 4, 9 }, { 9, 9 }, { 14, 2 }, { 15, 3 }, { 18, 5 }, { 20, 6 }, { 23, 8 }, { 25, 5 }, { 26, 3 }, { 29, 4 }, { 31, 7 }, { 34, 6 }, { 38, 7 }, { 39, 10 }, { 42, 3 }, { 43, 5 }, { 48, 3 }, { 50, 6 }, { 52, 8 }, { 54, 9 }, { 59, 6 }, { 60, 9 } };
	//std::vector < std::pair<int, int>> sprongen = { { 4, 0 }, { 4, 0 }, { 0, 1 }, { 2, 2 }, { 1, 1 }, { 2, 2 }, { 1, 0 }, { 0, 0 }, { 2, 1 }, { 1, 3 }, { 2, 0 }, { 0, 0 }, { 0, 3 }, { 2, 0 }, { 0, 2 }, { 4, 0 }, { 1, 3 }, { 1, 2 }, { 1, 1 }, { 4, 0 }, { 0, 3 } };

	// Maak map van alle hoogtes van de flats: KEY= positie x van flat, VALUE = hoogte
	std::map<int, int> hoogtes;
	for (auto p : flats) hoogtes[p.first] = p.second;

	// Initialiseer
	std::pair<int, int> pos = flats[0];
	int nSprongen = 0;

	// Main loop: voer sprong uit tot [vallen] of tot [successvol landen na laatste sprong]
	for (int i = 0; i < sprongen.size(); i++)
	{
		std::cout<<"Positie: (" << pos.first << "," << pos.second <<"). ";
		pos.first += sprongen[i].first+1;
		pos.second += sprongen[i].second;
		auto landingsHoogte = hoogtes.find(pos.first);
		if (landingsHoogte != hoogtes.end() && landingsHoogte->second <= pos.second )
		{
			pos.second = landingsHoogte->second;
			nSprongen++;
			std::cout << "Succesvolle "<<nSprongen<<"e sprong naar (" << pos.first << "," << pos.second<<")"<<std::endl;
		}
		else break;
	}
	if (nSprongen == sprongen.size()) std::cout << "success, nSprongen = " << nSprongen; else std::cout << "Fail na sprong: " << nSprongen<<", de kerstman valt dus bij sprong " << nSprongen + 1;
}
struct Node
{
	std::pair<int, int> pos;
	std::vector<Node*> nextFlats;
	int totalEnergyUsed;
};

bool kanVooruitLanden(std::pair<int, int> pos, std::pair<int, int> sprong, std::vector<std::pair<int, int>>& flats, std::map<int, int>& hoogtes)
{
	pos.first += sprong.first+1;
	pos.second += sprong.second;
	auto landingsHoogte = hoogtes.find(pos.first);
	if (landingsHoogte != hoogtes.end() && landingsHoogte->second <= pos.second)
	{
		//pos.second = landingsHoogte->second;
		return true;
	}
	else return false;
}
void VulBoom(Node* node, std::vector<std::pair<int, int>>& flats, std::vector < std::pair<int, int>>& basisSprongen, std::map<int, int>& hoogtes, std::vector<std::pair<int, int>>& solution)
{
	for (auto s : basisSprongen)
	{
		if (kanVooruitLanden(node->pos, s, flats, hoogtes))
		{
			std::pair<int, int> nextpos = { 0,0 };
			nextpos.first = node->pos.first + s.first + 1;
			auto landingsHoogte = hoogtes.find(nextpos.first);
			nextpos.second = landingsHoogte->second;
			int energy = s.first + s.second;

			if (node->nextFlats.size() == 0)
			{
				node->nextFlats.push_back(new Node({ nextpos, {}, node->totalEnergyUsed + energy }));
				std::cout << "Stap (virgin): " << s.first << "," << s.second << std::endl;
				solution.push_back({ s.first,s.second });
			}
			else
			{
				for (int i = 0; i < node->nextFlats.size(); i++)
				{
					if (node->nextFlats[i]->pos.first == nextpos.first && node->nextFlats[i]->pos.second == nextpos.second)
					{
						if (node->totalEnergyUsed + energy <= node->nextFlats[i]->totalEnergyUsed)
						{
							node->nextFlats.push_back(new Node({ nextpos, {}, node->totalEnergyUsed + energy }));
							std::cout << "Stap (added): " << s.first << "," << s.second << std::endl;
							solution.push_back({ s.first,s.second });

						}
					}
				}
			}
		}
	}
	for (auto p : node->nextFlats)
	{
		VulBoom(p, flats, basisSprongen, hoogtes, solution);
	}

}
void ToonResultaten(Node* node, std::pair<int, int>& endpoint)
{
	std::cout << "node (" << node->pos.first << "," << node->pos.second << ") -> ";
	if (node->pos.first == endpoint.first && node->pos.second == endpoint.second)
	{
		std::cout << "Total energy used: " << node->totalEnergyUsed<<std::endl;
	}
	else
	{
		//for (auto n : node->nextFlats)
		{
			//ToonResultaten(n, endpoint);
			ToonResultaten(node->nextFlats[0], endpoint);
		}
	}
	return;
}
void Infi2()
{
	// Test input
	//std::vector<std::pair<int, int>> flats = { {1,4},{3,8},{4,3},{5,7},{7,4},{10,3} };

	// Opdracht input
	std::vector<std::pair<int, int>> flats = { { 4, 9 }, { 9, 9 }, { 14, 2 }, { 15, 3 }, { 18, 5 }, { 20, 6 }, { 23, 8 }, { 25, 5 }, { 26, 3 }, { 29, 4 }, { 31, 7 }, { 34, 6 }, { 38, 7 }, { 39, 10 }, { 42, 3 }, { 43, 5 }, { 48, 3 }, { 50, 6 }, { 52, 8 }, { 54, 9 }, { 59, 6 }, { 60, 9 } };
	std::vector < std::pair<int, int>> basisSprongen = { {0,0},{0,1},{0,2 },{0,3},{0,4},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{3,0},{3,1},{4,0} };


	// Maak map van alle hoogtes van de flats: KEY= positie x van flat, VALUE = hoogte
	std::map<int, int> hoogtes;
	for (auto p : flats) hoogtes[p.first] = p.second;
	
	// Bouw een tree
	// Initialiseer
	Node* root = new Node({ flats[0], {}, 0});
	std::vector<std::pair<int, int>> solution;
	VulBoom(root, flats, basisSprongen, hoogtes,solution);
	ToonResultaten(root,flats.back());
	std::ofstream out("solution.txt", std::ios::out);
	std::cout << "[";
	out << '[';
	for (auto p : solution)
	{
		if ( p+1) == solution.end() )
		{
			std::cout << "[" << p.first << "," << p.second << "]]";
			//out << 't'<<p.first;
			out << '[' << p.first << ',' << p.second << ']'<<']';
		}
		else
		{
			std::cout << "[" << p.first << "," << p.second << "],";
			out << '[' << p.first << ',' << p.second << ']' << ',';
		}
	}





	

}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day17();
	Infi2();

	while (!_kbhit());
	return 0;
}

