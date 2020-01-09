#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"


void Day19a()
{
	IntCode_simple::IntCode IntComp("Resources/day19input.txt");
	//IntComp.Run();
	int count = 0;
	for (int y = 0; y < 50; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			count += IntComp.Run(x, y);
			IntComp.Reset();
		}
	}
	std::cout << "Count: " << count;
}
void Day19b_demo()
{
	// We can use IntComputer or load testdata from file.
	bool UseIntComputer = true;
	int boxSize = 10; // size of box that we want to fit
	std::vector<int> field;
	int fieldWidth = 0;
	int fieldHeight = 0;
	
	if (UseIntComputer==true)
	{
		std::cout << "Demo with IntComputer (you can set this at start of function), scanning a field of 200x200 for a box size 10 to fit in the beam.\npress Enter.\n";
		std::cin.get();
		fieldWidth = 200;
		fieldHeight = (int)((float)fieldWidth * 0.8f);
		IntCode_simple::IntCode IntComp("Resources/day19input.txt");
		// Populate the field
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)
			{
				int cell = IntComp.Run(x, y);
				IntComp.Reset();
				//if (cell == 1) std::cout << '#'; else std::cout << '.';
				field.push_back(cell);
			}
		}
	}
	else
	{
		std::cout << "Demo with test input from txt file (you can set this at start of function), scanning a field of 40x35 for a box size 10 to fit in the beam.\npress Enter.\n";
		std::cin.get();
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
		fieldHeight = field.size() / fieldWidth;
	}
	PrintBeamField(field, fieldWidth);

	std::queue<std::vector<int>> queue;
	for (int y = 0; y < boxSize; y++)
	{
		std::vector<int> line;
		for (int x = 0; x < fieldWidth; x++)
		{
			line.push_back(field[y * fieldWidth + x]);
		}
		queue.push(line);
	}
	int y_SantaSquare = 0;
	while (true)
	{
		int beamwidth_topline = GetBeamWidth(queue.front());
		int firstX_topline = GetFirstBeamXcoordinate(queue.front());
		int beamwidth_bottomline = GetBeamWidth(queue.back());
		int firstX_bottomline = GetFirstBeamXcoordinate(queue.back());
		std::cout << "y coord of topline: " << y_SantaSquare << '\n';
		std::cout << "beamwidth topline line: " << beamwidth_topline <<'\n';
		std::cout << "First x coord of topline beam: " << firstX_topline <<'\n';
		std::cout << "beamwidth bottom line: " << beamwidth_bottomline << '\n';
		std::cout << "First x coord of bottom line beam: " << firstX_bottomline << '\n';
		PrintQueue(queue);

		int x_SantaSquare = firstX_topline + beamwidth_topline - boxSize;
		if ((beamwidth_topline > boxSize) &&
			(firstX_bottomline == x_SantaSquare))
		{
			std::cout << "Square now fits,  topleft of square: x=" << x_SantaSquare << ", y=" << y_SantaSquare << '\n';
			std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (x_SantaSquare)+(y_SantaSquare)) << '\n'; // lastline correction -1 because of vertical linecount in routine used
			break;
		}
		if (!UseIntComputer) std::cin.get();
		ClearScreen();
		std::vector<int> line;
		for (int x = 0; x < fieldWidth; x++)
		{
			line.push_back(field[(y_SantaSquare + boxSize) * fieldWidth + x]);
		}
		y_SantaSquare++;
		queue.pop();
		queue.push(line);
	}
	return;
}

void Day19b_WithBinarySearch()
{
	// We can use IntComputer or load testdata from file.
	bool UseIntComputer = false;
	int boxSize = 100; // size of box that we need to fit
	int yFirstFit=-1; // these are the coordinates we are looking for
	int xFirstFit=-1;

	if (UseIntComputer)
	{
		
		IntCode_simple::IntCode IntComp("Resources/day19input.txt");
		
		// Run a Binary Search loop to establish at what minimum height y the SquareBox fits inside the beam
		// Initialize markers:
		int BS_y_highmark = 0;
		int BS_y_lowmark = 2000; // begin at last line of field minus size of box that needs to fit in
		int BS_y_test = BS_y_lowmark;

		// Run the BS loop	
		while (true)
		{
			if (BoxFitsAtHeight(BS_y_test, IntComp, boxSize))
			{
				BS_y_lowmark = BS_y_test;
				BS_y_test = BS_y_highmark + (BS_y_lowmark - BS_y_highmark) / 2;
				if (BS_y_highmark - BS_y_test == 1) // success condition for finding the highest fitting y coordinate (approach from bottom)
				{
					yFirstFit = BS_y_test;
					break;
				}
			}
			else
			{
				if (BS_y_lowmark - BS_y_test == 1) // success condition (approach from top)
				{
					yFirstFit = BS_y_lowmark;
					break;
				}
				BS_y_highmark = BS_y_test;
				BS_y_test = BS_y_test + (BS_y_lowmark - BS_y_test) / 2;
			}
		}
		int beamWidth = BeamWidthAtHeight(yFirstFit, IntComp);
		xFirstFit = GetFirstBeamXcoordinate(GetLine(yFirstFit, IntComp)) + beamWidth - boxSize;

		std::cout << "Topleft of first " << 10 << "x" << 10 << " square that fits inside the beam: x=" << xFirstFit << ", y=" << yFirstFit << '\n';
		std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (xFirstFit)+(yFirstFit)) << '\n';
	}
	else
	{
		std::cout << "\n\nExecuting function Day19b_WithBinarySearch().\n";
		std::cout << "Will run a binary search based algorithm on test input from txt file (you can set this at start of function), \nscanning a field of 40x35 for a box size 10 to fit in the beam.\npress Enter.\n";
		std::cin.get();
		boxSize = 10;
		// Load BeamField
		BeamField field;
		{
			std::vector<int> fieldVec_init;
			int fieldWidth_init;
			LoadBeamField(fieldVec_init, fieldWidth_init);
			field.fieldVec = fieldVec_init;
			field.fieldWidth = fieldWidth_init;
			field.fieldHeight = field.fieldVec.size() / field.fieldWidth;
		}
		PrintBeamField(field.fieldVec, field.fieldWidth);
		
				
		// Run a Binary Search loop to establish at what minimum height y the SquareBox fits inside the beam
		// Initialize markers:
		int BS_y_highmark = 0;
		int BS_y_lowmark  = (field.fieldHeight - 1) - boxSize; // begin at last line of field minus size of box that needs to fit in
		int BS_y_test = BS_y_lowmark;
		
		// Run the BS loop	
		while (true)
		{
			if (BoxFitsAtHeight(BS_y_test, field,boxSize))
			{
				BS_y_lowmark = BS_y_test;
				BS_y_test = BS_y_highmark + (BS_y_lowmark - BS_y_highmark) / 2;
				if (BS_y_highmark - BS_y_test == 1) // success condition for finding the highest fitting y coordinate (approach from bottom)
				{
					yFirstFit = BS_y_test;
					break;
				}
			}
			else
			{
				if (BS_y_lowmark - BS_y_test == 1) // success condition (approach from top)
				{
					yFirstFit = BS_y_lowmark;
					break;
				}
				BS_y_highmark = BS_y_test;
				BS_y_test = BS_y_test + (BS_y_lowmark - BS_y_test)/2;
			}
		}
		int beamWidth = BeamWidthAtHeight(yFirstFit,field);
		xFirstFit = GetFirstBeamXcoordinate(GetLine(yFirstFit, field)) + beamWidth - boxSize;

		std::cout << "Topleft of first "<< 10 << "x"<<10<<" square that fits inside the beam: x=" << xFirstFit << ", y=" << yFirstFit << '\n';
		std::cout << "So, the puzzle answer((x) * 10000 + (y)) = " << (10000 * (xFirstFit)+(yFirstFit)) << '\n';
	}
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day19b_demo();
	Day19b_WithBinarySearch();

	while (!_kbhit());
	return 0;
}


