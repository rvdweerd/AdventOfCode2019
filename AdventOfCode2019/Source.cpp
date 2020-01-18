#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
//#include "Day20HelperFunctions.h"

void DealIntoNew(std::vector<int>& deck)
{
	std::reverse(deck.begin(), deck.end());
	std::cout << "din      ; ";
	return;
}
void CutNCards(std::vector<int>& deck, int n)
{
	if (n > 0)
	{
		std::cout << "cut " << std::setw(5)<< n << "; ";
		auto it = deck.begin();
		for (int i=0; i<n;i++)
		{
			deck.push_back(*it);
			it = deck.erase(it);
		}
	}
	else if (n < 0)
	{
		std::cout << "cut " << std::setw(5)<< n << "; ";
		//CutNCards(deck, deck.size() + n);
		auto it = deck.begin();
		auto end = deck.size() + n;
		for (int i = 0; i < end; i++)
		{
			deck.push_back(*it);
			it = deck.erase(it);
		}
	}
	return;
}
void DealWithIncrement(std::vector<int>& deck, int n)
{
	int len = (int)deck.size();
	std::vector<int> newVec;  newVec.assign(deck.size(),0);
	std::cout << "dwi " << std::setw(5)<<n << "; ";
	std::vector<int> copy = deck;
	int offset = 0;
	for (int i = 0; i < copy.size(); i++)
	{
		if ((i * n) % copy.size() == 0 & i != 0)
		{
			offset++;
		}
		int index = (i * n) % copy.size()+offset;
		newVec[index] = copy[i];
	}
	deck = newVec;
	return;
}
void Day22Test()
{
	// Initialize the deck
	static constexpr int n = 10;// 007;
	std::vector<int> deck;
	for (size_t i = 0; i < n; i++)
	{
		deck.push_back(i);
	}

	std::vector<std::vector<int>> tests;

	for (int i = 1; i < 10; i++)
	{
		std::vector<int> copy = deck;
		DealWithIncrement(copy, i);
		std::cout <<  " yields: ";
		for (int v : copy)
		{
			std::cout << v << ", ";
		}
		std::cout << "\n";
	}
}


void Day22a_literal()
{
	// Initialize the deck
	static constexpr int n = 10;// 007;
	std::vector<int> deck;
	for (size_t i = 0; i < n; i++)
	{
		deck.push_back(i);
	}
	
	// Execute all intructions in the input file
	{
		std::ifstream in("Resources/day22input.txt");
		for (std::string str; std::getline(in, str, '\n');)
		{
			std::istringstream stream(str);
			std::vector<std::string> words;
			for (std::string word; stream >> word;)
			{
				words.push_back(word);
			}

			if (words[0][0] == 'c') // instruction: cut
			{
				int n = stoi(words.back());
				CutNCards(deck, n);
			}
			else if (words[1][0] == 'w') // instruction: deal with increment 
			{
				int n = stoi(words.back());
				DealWithIncrement(deck, n);
			}
			else if (words[1][0] == 'i') // instruction: deal into new stack
			{
				DealIntoNew(deck);
			}
			else
			{
				std::cout << "error reading instructions.\n";
			}
			std::cout << "  => New dec: [ ";
			for (int i = 0; i < min(20, deck.size()); i++)
			{
				std::cout << std::setw(5) << deck[i] << ", ";
			}
			std::cout << " ... ] => ";
			std::cout << "The position of card "<<2019%deck.size()<<" is: " << int(std::find(deck.begin(), deck.end(), 2019%deck.size()) - deck.begin()) << "\n";
		}

		size_t index2019 = (std::find(deck.begin(), deck.end(), 2019 % deck.size()) - deck.begin());
		std::cout << "After shuffeling, the position of card " << 2019 % deck.size() << " is: " << index2019 << '\n';
		std::cout << "The " << 2019 % deck.size() << "th card in the shuffled deck is: " << deck[2019%deck.size()];
	}
	
	
	
	return;
}
enum class ShuffleType
{
	DIN,
	DWI,
	CUT
};
struct MOVE
{
	ShuffleType sType;
	int n;
};
int NewIndexAfterShuffling(const int deckSize, const int cardNumber, const std::vector<MOVE>& moves)
{
	int newIndex = cardNumber;
	for (MOVE m : moves)
	{
		switch (m.sType)
		{
		case ShuffleType::DIN:
			newIndex = (deckSize - newIndex - 1) % deckSize;
			break;
		case ShuffleType::CUT:
			newIndex = (newIndex - m.n + deckSize) % deckSize;
			break;
		case ShuffleType::DWI:
			newIndex = (newIndex * m.n ) % deckSize;
			break;
		}
	}
	return newIndex;
}
void Day22a_clean()
{
	// Parse all intructions from the input file into a vector
	std::vector<MOVE> moves;
	{
		std::ifstream in("Resources/day22input.txt");
		for (std::string str; std::getline(in, str, '\n');)
		{
			// retrieve step integer from instruction (if present)
			int n = 0;
			if (str.back() >= '0' && str.back() <= '9')
			{
				size_t i = str.rfind(' ');
				n = stoi(str.substr(i));
			}
			// push back intstruction onto moves vector
			if (str.find("cut")!=std::string::npos) // instruction: cut
			{
				moves.push_back({ ShuffleType::CUT,n });
			}
			else if (str.find("wit") != std::string::npos) // instruction: deal with increment 
			{
				moves.push_back({ ShuffleType::DWI,n });
			}
			else if (str.find("int") != std::string::npos) // instruction: deal into new stack
			{
				moves.push_back({ ShuffleType::DIN,0 });
			}
			else
			{
				std::cout << "error reading instructions.\n";
			}	
		}
	}

	// Apply moves
	int deckSize = 10007;
	int cardNumber = 2019;
	int newIndex = NewIndexAfterShuffling(deckSize, cardNumber, moves);
	std::cout << "After " << moves.size() << " sufffle moves, the new position of card " << cardNumber << " is: " << newIndex;
	return;
}

int main()
{
	// Instruction: load data in appropriate .txt input file and run the function associated with a specific day
	// So, for example, to run Day 7 challenge:
	// --> save data to "Resources/day7ainput.txt" and "Resources/day7binput.txt"
	// --> run the functions Day7a(); and/or Day7b(); in main()
	//Day22Test();
	Day22a_clean();
	


	while (!_kbhit());
	return 0;
}


