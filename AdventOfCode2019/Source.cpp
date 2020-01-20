#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
//#include "Day20HelperFunctions.h"
#include "IntCode_net.h"

void Day23a()
{
	// Initialize the computer network
	std::map<int, IntCode_net*> compNet;
	for (int i = 0; i < 50; i++)
	{
		compNet.emplace( i , new IntCode_net("Resources/day23input.txt"));
		std::vector<long long int> retVec = compNet[i]->Run({ i });
	}

	// Run
	// Principle: loop the computers to pass -1 values. When a response is received, put these on a global queue
	// Handle queue responses (and add any subsequent responses to the queue) until the queue is empty,
	// then go back to looping with -1 values.
	std::queue<long long int> queue;
	bool CeaseOps = false;
	int instructionCount = 0;
	while (!CeaseOps)
	{
		for (int i = 0; i < 50; i++) // loop the network for responses
		{
			{
				std::cout << "Passing to comp: " << i << ", instruction X=" << -1 << '\n';
				std::cout << "Receiving back : ";
				std::vector<long long int> retVec = compNet[i]->Run({ -1 });

				// Add responses to the queue (note: we assume output adheres to instruction format [id,X,Y]
				for (auto v : retVec) queue.push(v);
			}

			// Handle all queue instructions until empty
			while (!queue.empty())
			{
				// Retrieve instruction at front of queue [id,X,Y]
				int compId = (int)queue.front(); queue.pop(); instructionCount++;
				long long int instr1 = queue.front(); queue.pop();
				long long int instr2 = queue.front(); queue.pop();
				if (compId == 255)
				{
					std::cout << "Packet sent to address 255. Y value = " << instr2;
					CeaseOps = true;
					break;
				}
				std::cout << "Passing to comp: " << compId << ", instruction X=" << instr1 << ", Y=" << instr2 << '\n';
				std::cout << "Receiving back : ";
				std::vector<long long int> retVec = compNet[compId]->Run({ instr1, instr2 });
				for (auto v : retVec) queue.push(v);
			}
		}
		if (CeaseOps) break;
	}
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day22Test();
	//Day22a_literal();
	//Day22a_clean1();
	//Day22a_clean2();
	Day23a();
	
	while (!_kbhit());
	return 0;
}


