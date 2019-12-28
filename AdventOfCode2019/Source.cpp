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


int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day17();
	
	while (!_kbhit());
	return 0;
}

