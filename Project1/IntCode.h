#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class IntCode
{
public:
	IntCode(std::string fileName, int phaseSetting=0)
	{
		std::ifstream in(fileName);
		while (!in.eof())
		{
			std::string str;
			for (char ch = in.get(); ch != ',' && !in.eof(); ch = in.get())
			{
				str += ch;
			}
			originalCodeVec.push_back(std::stoi(str));
		}
		runCodeVec = originalCodeVec;
	}
	void Reset()
	{
		runCodeVec = originalCodeVec;
	}
	void ResetWithPhase(int phaseSetting)
	{

	}
	void Run()
	{
		int increment = 0;
		for (int i = 0; runCodeVec[i] != 99; i += increment)
		{
			GetParameters(i);
			if (opcode == 1) // add
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				int writePos = runCodeVec[i + 3];
				runCodeVec[writePos] = val1 + val2;
				increment = 4;
			}
			else if (opcode == 2) // multiply
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				int writePos = runCodeVec[i + 3];
				runCodeVec[writePos] = val1 * val2;
				increment = 4;
			}
			else if (opcode == 3) // input
			{
				int k;
				std::cout << "Input:"; std::cin >> k;
				runCodeVec[runCodeVec[i + 1]] = k;
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				std::cout << val1 << std::endl;
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				std::cout << val1;
				increment = 2;
			}
			else if (opcode == 5) // jump-if-true
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				if (val1 != 0)
				{
					i = val2;
					increment = 0;
				}
				else
				{
					increment = 3;
				}
			}
			else if (opcode == 6) // jump-if-false
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				if (val1 == 0)
				{
					i = val2;
					increment = 0;
				}
				else
				{
					increment = 3;
				}
			}
			else if (opcode == 7) // less-than
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				int writePos = runCodeVec[i + 3];

				runCodeVec[writePos] = (val1 < val2 ? 1 : 0);
				increment = 4;
			}
			else if (opcode == 8) // equals
			{
				int val1 = (C == 0 ? runCodeVec[runCodeVec[i + 1]] : runCodeVec[i + 1]);
				int val2 = (B == 0 ? runCodeVec[runCodeVec[i + 2]] : runCodeVec[i + 2]);
				int writePos = runCodeVec[i + 3];
				runCodeVec[writePos] = (val1 == val2 ? 1 : 0);
				increment = 4;
			}
			else
			{
				std::cout << "error.";
			}
		}
	}
	int Run(int input)
	{

	}
	std::vector<int>& GetRunCodeVectorReference()
	{
		return runCodeVec;
	}
private:
	void GetParameters(int index)
	{
		int val = runCodeVec[index];
		opcode = val % 100;
		C = (val / 100) % 10;
		B = (val / 1000) % 10;
		A = (val / 10000) % 10;
	}
private:
	int A = 0;
	int B = 0;
	int C=0;
	int opcode=0;
	std::vector<int> runCodeVec;
	std::vector<int> originalCodeVec;
};
