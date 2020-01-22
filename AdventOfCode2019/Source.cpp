#pragma once
#include "IncludesUsed.h"

void Day25a()
{
	// Run the manually pre-compiled script to pick up all the items in the maze and get to the pressure sensor door
	Day25::IntCode_charLL compASCII("Resources/day25input.txt");
	std::vector<std::string> allInstr;
	std::string input;
	std::ifstream in("instructions.txt");
	while (std::getline(in, input))
	{
		ClearScreen();
		compASCII.Run(input+'\n');
	}

	// Put the full inventory in a vector
	std::string test = compASCII.Run("inv\n");
	std::vector<std::string> inventoryFull;
	auto it = test.find('-');
	while (true)
	{
		bool complete = false;
		test.erase(0, it + 2);
		it = test.find('-');
		if (it == std::string::npos)
		{
			it = test.find('\n')+1;
			complete = true;
		}
		inventoryFull.push_back(test.substr(0, it - 1));
		if (complete) break;
	}

	// Create (with recursion) vector of vector<string> with all subsets of the inventory
	std::vector<std::vector<std::string>> inventorySubset;
	SubSetRec(inventorySubset, {}, inventoryFull);
	
	// Drop all items
	std::string dropAllCommand = GenerateInputString("drop", inventoryFull);
	compASCII.Run(dropAllCommand);
	
	// Try all variations of items and move north until the door opens
	std::string negative = "ejected back";
	std::string answer = negative;
	int subSetIndex = -1;
	while (true)
	{
		// proceed to next subset & pick up items in that subset
		subSetIndex++;
		std::string takeCommand = GenerateInputString("take", inventorySubset[subSetIndex]);
		std::string dropCommand = GenerateInputString("drop", inventorySubset[subSetIndex]);
		compASCII.Run(takeCommand);

		// try to open the door
		answer = compASCII.Run("north\n");
		if (answer.find(negative) == std::string::npos)
		{
			break;
		}

		//drop all items
		compASCII.Run(dropCommand);
	}
	GameManualMode(compASCII);
	return;
}
int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day25a();
	
	while (!_kbhit());
	return 0;
}