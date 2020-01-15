#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
//#include "Day20HelperFunctions.h"

void Day21a()
{
	Day21::IntCode_char compu("Day21input.txt");
	std::string s1;
	s1 += "NOT C J\n";
	s1 += "AND D J\n";
	s1 += "NOT A T\n";
	s1 += "OR T J\n";
	s1 += "WALK\n";
	compu.Run(s1);
	std::cout << '\n';
}
void Day21b()
{
	Day21::IntCode_char compu("Day21input.txt");
	std::string s1;
	s1 += "NOT A J\n";
	s1 += "NOT B T\n";
	s1 += "OR T J\n";
	s1 += "NOT C T\n";
	s1 += "OR T J\n";
	s1 += "AND D J\n";
	s1 += "NOT E T\n";
	s1 += "NOT J T\n";
	s1 += "OR E T\n";
	s1 += "OR H T\n";
	s1 += "AND T J\n";
	s1 += "RUN\n";
	compu.Run(s1);
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day21a();
	Day21b();

	while (!_kbhit());
	return 0;
}


