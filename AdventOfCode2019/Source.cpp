#pragma once
#include "IncludesUsed.h"
//#include "PastDaysSolutions.h"
//#include "Day20HelperFunctions.h"

void Day22a_demo()
{
	// Initialize the deck
	static constexpr int n = 10007;
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
			std::cout << "The position of card " << 2019 % deck.size() << " is: " << int(std::find(deck.begin(), deck.end(), 2019 % deck.size()) - deck.begin()) << "\n";
		}

		size_t index2019 = (std::find(deck.begin(), deck.end(), 2019 % deck.size()) - deck.begin());
		std::cout << "After shuffeling, the position of card " << 2019 % deck.size() << " is: " << index2019 << '\n';
		std::cout << "The " << 2019 % deck.size() << "th card in the shuffled deck is: " << deck[2019 % deck.size()]<<"\n\n";
	}
	return;
}
void Day22a()
{
	// Parse all intructions from the input file into a vector
	std::vector<MOVE> moves;
	LoadMoves(moves);

	LL N = 10007;
	LL startIndex = 2019;
	LL newIndex = ApplyMoves(startIndex, N, moves);

	std::cout << "After " << moves.size() << " sufffle moves, the new position of card " << startIndex << " is: " << newIndex<< "\n\n";
}
void Day22b()
{
	// For this challenge I had to revert to the Subreddit for aoc2019 since my modular math was a bit rusty.
	// Key resources to help solve this were:
	// => Wikipedia on Modular Arithmaticand Modular Multiplicative Inverse
	// => CP algorithms on Binary Exponentiation : https://cp-algorithms.com/algebra/binary-exp.html
	// => CP algorithms on the Extended Euclidean Algo : https://cp-algorithms.com/algebra/extended-euclid-algorithm.html
	// => cs.stackexchange on Modular Multiplicationand avoiding int64 overflow : https://cs.stackexchange.com/questions/77016/modular-multiplication
	
	// Parse all intructions from the input file into a vector
	std::vector<MOVE> moves;
	LoadMoves(moves);

	// Define the tranformation function F(i) as the function that return the new index of position i after applying all moves.
	// We need to find the linear composition of the inversed transformation function 
	//
	//		F_inv(i) = ALPHA * i + BETA
	//
	// In order to calculate ALPHA and BETA, we will apply F_inv(i) to our endposition X=2020 twice. This yields:
	//		Y = F_inv(X)
	//		Z = F_inv(Y*) = F_inv(F_inv(X))
	// 
	// With these results, we can calculate ALPHA and BETA, remembering that all is under modulo of N
	//		ALPHA = (Y-Z) * MMI(X-Y+N,N) % N
	//		BETA  = (Y-ALPHA*X) % N
	// (where MMI = modular multiplicative inverse ax==1(mod M) )
	LL X = 2020;
	LL N = 119315717514047;

	// Apply the moves in reverse order to the endposition X referred to in the question (2020) to the deck size N
	std::vector<MOVE> moves_reversed(moves);
	std::reverse(moves_reversed.begin(), moves_reversed.end());
	LL Y = ApplyReverseMoves(X, N, moves_reversed);
	LL Z = ApplyReverseMoves(Y, N, moves_reversed);
	LL ALPHA = mul_mod((Y - Z), MMI(X - Y + N, N), N) % N;
	LL BETA = (Y - mul_mod(ALPHA, X, N)) % N;

	// Now we are asked to backtrack an insane amount of nShuffles
	// Writing out the inverse transformation function n times shows a pattern that contains a geomatric series:
	//
	// F_inv^n(i) = ALPHA^n * i + ALPHA^(n-1)*BETA + APLHA^(n-2)*BETA + ... + ALPHA^(0)*BETA = 
	//
	//			  = ALPHA^n * i + (ALPHA^n-1) / (ALPHA-1) * BETA
	//
	// And let's not forget this is still in bloody modulo N
	LL nShuffles = 101741582076661;
	LL startIndex =	(	mul_mod(binpow_undermod(ALPHA, nShuffles, N), X, N) + 
						mul_mod(mul_mod((binpow_undermod(ALPHA, nShuffles, N) - 1), MMI(ALPHA - 1, N), N), BETA, N) % N  
					) % N;

	// Print results
	std::cout << "The card that ends up in position 2020 after " << nShuffles << " shuffles is: " << startIndex;
	std::cout << "\nNow get back to work!";
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
	Day22a_demo();
	Day22a();
	Day22b();

	while (!_kbhit());
	return 0;
}


