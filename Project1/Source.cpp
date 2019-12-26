#include "IncludesUsed.h"
#include "PastDaysSolutions.h"

std::vector<int> FFTnPattern(std::vector<int> basicPattern, int n)
{
	// basicPattern size: [a]  --> return vector size: [a*n];
	std::vector<int> tempVec;
	int a = basicPattern.size();
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < n; j++)
		{
			tempVec.push_back(basicPattern[i]);
		}
	}
	return tempVec;
}
std::vector<std::vector<int>> FFTBuildMaskMatrix(std::vector<int> basicPattern, int FFTsize)
{
	// basicPattern size: [a] --> return Matrix size [ FFT x [a ... a*FFTsize] ]
	std::vector<std::vector<int>> tempMatrix;
	for (int i = 1; i <= FFTsize; i++)
	{
		tempMatrix.push_back(FFTnPattern(basicPattern, i));
	}
	return tempMatrix;
}
int FFTrow(std::vector<int> signal, std::vector<int> maskVector)
{
	int temp = 0;
	const int signalLength = signal.size();
	const int maskLength = maskVector.size();
	for (int i = 0; i < signalLength; i++)
	{
		temp += signal[i] * maskVector[(i+1) % maskLength];
	}
	temp = std::abs(temp);
	return LastDigit(temp);
}
std::vector<int> FFT(std::vector<int> signal, std::vector<std::vector<int>> maskMatrix, int n=1)
{
	std::vector<int> tempVec;
	int nDigits = signal.size();
	for (int numberOfTransforms = 0; numberOfTransforms < n; numberOfTransforms++)
	{
		tempVec.clear();
		for (int i = 0; i < nDigits; i++)
		{
			tempVec.push_back(FFTrow(signal, maskMatrix[i]));
		}
		signal = tempVec;
	}
	return tempVec;
}
void Day16a()
{
	// Load data from file
	std::vector<int> signal;
	{
		std::ifstream in("day16input.txt");
		char ch;
		for (in >> ch; !in.eof(); in >> ch)
		{
			signal.push_back(int(ch - '0'));
		}
	}

	// Initialization
	std::vector<int> basic_pattern = { 0,1,0,-1 };
	std::vector<std::vector<int>> maskMatrix = FFTBuildMaskMatrix(basic_pattern, signal.size());

	// Apply FFT 100 times
	std::vector<int> result = FFT(signal, maskMatrix, 100);
	std::cout << "The first eight digits of the result: ";
	for (int i = 0; i < 8; i++) std::cout << result[i];
}
void Day16b()
{
	// Load data from file
	std::vector<int> signal_orig;
	int offset = 0;
	{
		std::ifstream in("day16input.txt");
		int count = 0;
		char ch;
		for (in >> ch; !in.eof(); in >> ch)
		{
			signal_orig.push_back(int(ch - '0'));
			if (count < 7)
			{
				offset += std::pow(10, 7 - count-1)*int(ch-'0');
				count++;
			}
		}
	}
	// Fill signal vetor x10.000
	std::vector<int> signal_full;
	for (int i = 0; i < 10000;i++)
	{
		std::copy(signal_orig.begin(), signal_orig.end(), std::back_inserter(signal_full));
	}
	std::vector<int> sub_signal(signal_full.begin() + offset, signal_full.end());
	int sub_signalLength = sub_signal.size();

	// FFT on sub-signal
	std::vector<int> temp;
	for (int n = 0; n < 100; n++)
	{
		temp.clear();
		temp.push_back(sub_signal.back());
		for (int i = sub_signalLength - 1; i > 0; i--)
		{
			int elem = temp.back() + sub_signal[i - 1];
			temp.push_back(elem % 10);
		}
		std::reverse(temp.begin(), temp.end());
		sub_signal.swap(temp);
	}
	std::cout << "The first eight digits of the result: ";
	for (int i = 0; i < 8; i++) std::cout << sub_signal[i];
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "day7ainput.txt" and "day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	Day16b();
	

	while (!_kbhit());
	return 0;
}

