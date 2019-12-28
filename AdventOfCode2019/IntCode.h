#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cassert>

class IntCode
{
public:
	IntCode(std::string fileName)
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

		// setup winapi consolescreen
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) std::cout << "ERROR INITIALIZING WINAPI";

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) std::cout << "ERROR INITIALIZING WINAPI";
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) std::cout << "ERROR INITIALIZING WINAPI";

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	}
	void SetFirstDigit(int n)
	{
		runCodeVec[0] = n;
	}
	void Reset()
	{
		runCodeVec = originalCodeVec;
		code_index = 0;
	} 
	void Run() // Run in manual mode (I/O by user)
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
				int k;
				//std::cin.ignore(1000, '\n');
				//std::cin.clear();

				std::cout << "Input:"; 
				std::cin >> k;
				writeVal<char>(code_index + 1, C, (char)k);
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				//long long int val1 = readVal<long long int>(code_index + 1, C);
				char val1 = readVal<char>(code_index + 1, C);
				std::cout << val1;// << ",";
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
			}
		}
	} 
	template <typename E>	// Run automated Int computer commands (I/O connected with function caller)
	E Run(E input1, E input2 = 0) 
	{
		int increment = 0;
		for (/*code_index = 0*/; (int)runCodeVec[code_index] != 99; code_index += increment)
		{
			GetParameters(code_index);
			if (opcode == 1) // add
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, val1 + val2);
				increment = 4;
			}
			else if (opcode == 2) // multiply
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, val1 * val2);
				increment = 4;
			}
			else if (opcode == 3) // input
			{
				int k;
				if (ballPosX > padPosX) k = 1;
				else if (ballPosX < padPosX) k = -1;
				else k = 0;
				//PrintTextToConsole("Input: ", { 0,30 });
				//std::cin >> k;
				PrintTextToConsole("Input: ", { 0,30 }); std::cout << k;
				writeVal<E>(code_index + 1, C, k);
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				E val1 = readVal<E>(code_index + 1, C);
				//std::cout << val1 << ",";
				//increment = 2;
				code_index += 2;
				//if (val1 == 3||true) { std::cout << "value 3 retured, code_index=" << code_index; }
				return val1;
			}
			else if (opcode == 5) // jump-if-true
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
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
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
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
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, (val1 < val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 8) // equals
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, (val1 == val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 9) // adjust relative base
			{
				E val1 = readVal<E>(code_index + 1, C);
				relativeBase += int(val1);
				increment = 2;
			}
			else
			{
				std::cout << "error.";
			}
		}
		return (int)runCodeVec[code_index];
	} 
	template <typename E>	// Adjustments made to use Intode for Day15 challeng
	E Run15(E input1, E input2 = 0) 
	{
		int increment = 0;
		for (/*code_index = 0*/; (int)runCodeVec[code_index] != 99; code_index += increment)
		{
			GetParameters(code_index);
			if (opcode == 1) // add
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, val1 + val2);
				increment = 4;
			}
			else if (opcode == 2) // multiply
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, val1 * val2);
				increment = 4;
			}
			else if (opcode == 3) // input
			{
				writeVal<E>(code_index + 1, C, input1);
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				E val1 = readVal<E>(code_index + 1, C);
				//std::cout << val1 << ",";
				//increment = 2;
				code_index += 2;
				//if (val1 == 3||true) { std::cout << "value 3 retured, code_index=" << code_index; }
				if (val1 == 2) 
				{ 
					int eggeq = 0; 
				}
				return val1;
			}
			else if (opcode == 5) // jump-if-true
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
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
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
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
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, (val1 < val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 8) // equals
			{
				E val1 = readVal<E>(code_index + 1, C);
				E val2 = readVal<E>(code_index + 2, B);
				writeVal<E>(code_index + 3, A, (val1 == val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 9) // adjust relative base
			{
				E val1 = readVal<E>(code_index + 1, C);
				relativeBase += int(val1);
				increment = 2;
			}
			else
			{
				std::cout << "error.";
				assert(false);
			}
		}
		return -999;
	}
	int Run(std::vector<char> instructions)
	{
		int vectorIndex = 0;
		int increment = 0;
		for (/*code_index = 0*/; (int)runCodeVec[code_index] != 99; code_index += increment)
		{
			GetParameters(code_index);
			if (opcode == 1) // add
			{
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
				writeVal<int>(code_index + 3, A, val1 + val2);
				increment = 4;
			}
			else if (opcode == 2) // multiply
			{
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
				writeVal<int>(code_index + 3, A, val1 * val2);
				increment = 4;
			}
			else if (opcode == 3) // input
			{
				//int k;
				std::cout << "Input read: " << instructions[vectorIndex];
				//std::cout << instructions[vectorIndex];
				writeVal<int>(code_index + 1, C, (int)instructions[vectorIndex]);
				vectorIndex++;
				increment = 2;
			}
			else if (opcode == 4) // output
			{
				int val1 = readVal<int>(code_index + 1, C);
				std::cout << (int)val1<< ",";
				increment = 2;
				//code_index += 2;
				//if (val1 == 3||true) { std::cout << "value 3 retured, code_index=" << code_index; }
				//return val1;
			}
			else if (opcode == 5) // jump-if-true
			{
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
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
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
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
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
				writeVal<int>(code_index + 3, A, (val1 < val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 8) // equals
			{
				int val1 = readVal<int>(code_index + 1, C);
				int val2 = readVal<int>(code_index + 2, B);
				writeVal<int>(code_index + 3, A, (val1 == val2 ? 1 : 0));
				increment = 4;
			}
			else if (opcode == 9) // adjust relative base
			{
				int val1 = readVal<int>(code_index + 1, C);
				relativeBase += int(val1);
				increment = 2;
			}
			else
			{
				std::cout << "error.";
			}
		}
		return (int)runCodeVec[code_index];
	}

	std::vector<long long int>& GetRunCodeVectorReference()
	{
		return runCodeVec;
	}
	void PrintTextToConsole(std::string text, COORD coord)
	{
		SetConsoleCursorPosition(hStdOut, coord);
		std::cout << text;
	}
	void PrintSymbolToConsole(int id, COORD coord)
	{
		SetConsoleCursorPosition(hStdOut, coord);
		switch (id)
		{
		case 0:
			std::cout << " ";
			break;
		case 1:
			std::cout << "W";
			break;
		case 2:
			std::cout << "B";
			break;
		case 3:
			std::cout << "=";
			PrintTextToConsole("Pad position: (", { 0,28 }); std::cout << coord.X << "," << coord.Y << ")";
			padPosX = coord.X;
			break;
		case 4:
			std::cout << "O";
			PrintTextToConsole("Ball position: (", { 0,27 }); std::cout << coord.X << "," << coord.Y << ")";
			ballPosX = coord.X;
			break;
		std::cout << "X";
		}
		return;
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
	int code_index=0;
	int A = 0;
	int B = 0;
	int C=0;
	int opcode=0;
	int relativeBase = 0;
	std::vector<long long int> runCodeVec;
	std::vector<long long int> originalCodeVec;
	int ballPosX;
	int padPosX;

private:
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

};
