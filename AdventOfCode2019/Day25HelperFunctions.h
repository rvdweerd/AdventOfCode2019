#pragma once
#include "IntCode_charLL.h"
#include <algorithm>
#include <iostream>

void GameManualMode(Day25::IntCode_charLL& compASCII)
{
	while (1)
	{
		std::string input;
		std::getline(std::cin, input);
		if (input == "quit") break;

		std::replace(input.begin(), input.end(), ';', '\n');
		if (input.size() > 0)
		{
			if (input.back() != '\n') input += '\n';
		}
		if (input.substr(0, 2) != "//")
		{
			ClearScreen();
			compASCII.Run(input);
		}
	}
	return;
}
void Record(std::string filename)
{
	Day25::IntCode_charLL compASCII("Resources/day25input.txt");
	std::ofstream out("instructions.txt");
	while (1)
	{
		std::string input;
		std::getline(std::cin, input);
		if (input == "quit") break;

		std::replace(input.begin(), input.end(), ';', '\n');
		if (input.size() > 0)
		{
			if (input.back() != '\n') input += '\n';
		}
		out << input;
		if (input.substr(0, 2) != "//")
		{
			ClearScreen();
			compASCII.Run(input);
		}
	}
	out.close();
	return;
}
void SubSetRec(std::vector<std::vector<std::string>>& inventorySubset, std::vector<std::string> sofar, std::vector<std::string> inventory)
{
	if (inventory.size() == 0)
	{
		inventorySubset.push_back(sofar);
		return;
	}
	else
	{
		std::string nextItem = inventory[0];
		inventory.erase(inventory.begin());
		SubSetRec(inventorySubset, sofar, inventory);
		sofar.push_back(nextItem);
		SubSetRec(inventorySubset, sofar, inventory);
	}
}
std::string GenerateInputString(std::string command, std::vector<std::string> items)
{
	std::string output;
	for (auto i : items)
	{
		output += command;
		output += ' ';
		output += i;
		output += '\n';
	}
	return output;
}

