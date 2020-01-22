#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

namespace Day25
{
	class IntCode_charLL
	{
	public:
		IntCode_charLL(std::string fileName)
		{
			std::ifstream in(fileName);
			while (!in.eof())
			{
				std::string str;
				for (char ch = in.get(); ch != ',' && !in.eof(); ch = in.get())
				{
					str += ch;
				}
				originalCodeVec.push_back(std::stoll(str));
			}
			originalCodeVec.resize(size_t(1e5), 0);
			runCodeVec = originalCodeVec;
		}
		void Reset()
		{
			runCodeVec = originalCodeVec;
			code_index = 0;
			relativeBase = 0;
		}
		void Run()
		{
			int increment = 0;
			for (code_index = 0; (int)runCodeVec[code_index] != 99; code_index += increment)
			{
				GetParameters(code_index);
				if (opcode == 1) // add
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, val1 + val2);
					increment = 4;
				}
				else if (opcode == 2) // multiply
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, val1 * val2);
					increment = 4;
				}
				else if (opcode == 3) // input
				{
					char ch;
					std::cout << "Input:"; std::cin >> ch;
					writeVal<long long int>(code_index + 1, C, ch);
					increment = 2;
				}
				else if (opcode == 4) // output
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					if (val1 >=0 && val1< 256) std::cout << (char)val1;
					else std::cout << val1;
					increment = 2;
				}
				else if (opcode == 5) // jump-if-true
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					if (val1 != 0)
					{
						code_index = int(val2);
						increment = 0;
					}
					else
					{
						increment = 3;
					}
				}
				else if (opcode == 6) // jump-if-false
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					if (val1 == 0)
					{
						code_index = int(val2);
						increment = 0;
					}
					else
					{
						increment = 3;
					}
				}
				else if (opcode == 7) // less-than
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, (val1 < val2 ? 1 : 0));
					increment = 4;
				}
				else if (opcode == 8) // equals
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, (val1 == val2 ? 1 : 0));
					increment = 4;
				}
				else if (opcode == 9) // adjust relative base
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					relativeBase += int(val1);
					increment = 2;
				}
				else
				{
					std::cout << "error.";
					return;
				}
			}
			return;
		}
		std::string Run(std::string inputInstructions)
		{
			std::string outputString;
			int instructionIndex = 0;
			int increment = 0;
			for (; (int)runCodeVec[code_index] != 99; code_index += increment)
			{
				GetParameters(code_index);
				if (opcode == 1) // add
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, val1 + val2);
					increment = 4;
				}
				else if (opcode == 2) // multiply
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, val1 * val2);
					increment = 4;
				}
				else if (opcode == 3) // input
				{
					if (instructionIndex == inputInstructions.size())
					{
						return outputString;
					}
					//std::cout << inputInstructions[instructionIndex];
					writeVal<int>(code_index + 1, C, inputInstructions[instructionIndex++]);
					increment = 2;
				}
				else if (opcode == 4) // output
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					if (val1 >= 0 && val1 < 256)
					{
						std::cout << (char)val1;
						outputString += (char)val1;
					}
					else
					{
						std::cout << val1;
						outputString += std::to_string(val1);
					}
					increment = 2;
					//code_index += 2;
					//return;
				}
				else if (opcode == 5) // jump-if-true
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					if (val1 != 0)
					{
						code_index = int(val2);
						increment = 0;
					}
					else
					{
						increment = 3;
					}
				}
				else if (opcode == 6) // jump-if-false
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					if (val1 == 0)
					{
						code_index = int(val2);
						increment = 0;
					}
					else
					{
						increment = 3;
					}
				}
				else if (opcode == 7) // less-than
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, (val1 < val2 ? 1 : 0));
					increment = 4;
				}
				else if (opcode == 8) // equals
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					long long int val2 = readVal<long long int>(code_index + 2, B);
					writeVal<long long int>(code_index + 3, A, (val1 == val2 ? 1 : 0));
					increment = 4;
				}
				else if (opcode == 9) // adjust relative base
				{
					long long int val1 = readVal<long long int>(code_index + 1, C);
					relativeBase += int(val1);
					increment = 2;
				}
				else
				{
					std::cout << "error.";
					return outputString;
				}
			}
			return outputString;
		}


		std::vector<long long int>& GetRunCodeVectorReference()
		{
			return runCodeVec;
		}
	private:
		void GetParameters(int index)
		{
			int val = int(runCodeVec[index]);
			opcode = val % 100;
			C = (val / 100) % 10;
			B = (val / 1000) % 10;
			A = (val / 10000) % 10;
		}
		template <typename E>
		E readVal(int index, int mode)
		{
			switch (mode)
			{
			case 0: // position mode
				return (E)runCodeVec[(int)runCodeVec[index]];
				break;
			case 1: //  immediate mode
				return (E)runCodeVec[index];
				break;
			case 2: // relative mode
				return (E)runCodeVec[(int)runCodeVec[index] + relativeBase];
				break;
			}
			std::cout << "Error, no parameter mode found for readVal()";
			return -1;
		}
		template <typename E>
		void writeVal(int index, int mode, E value)
		{
			int writePos = 0;
			switch (mode)
			{
			case 0: // position mode
				writePos = int(runCodeVec[index]);
				runCodeVec[writePos] = value;
				return;
				break;
			case 1: //  immediate mode
				writePos = int(runCodeVec[index]);
				runCodeVec[writePos] = value;
				std::cout << "Error, write instruction in immediate mode (continued as if in position mode)\n";
				return;
				break;
			case 2: // relative mode
				writePos = int(runCodeVec[index] + relativeBase);
				runCodeVec[writePos] = value;
				return;
				break;
			}
			std::cout << "Error, no parameter mode found for writeVal()\n";
			return;
		}

	private:
		int code_index = 0;
		int A = 0;
		int B = 0;
		int C = 0;
		int opcode = 0;
		int relativeBase = 0;
		std::vector<long long int> runCodeVec;
		std::vector<long long int> originalCodeVec;
	};

}