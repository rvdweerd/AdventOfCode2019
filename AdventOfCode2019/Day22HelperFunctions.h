#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <sstream>

void DealIntoNew(std::vector<int>& deck)
{
	// support funtion to apply a DIN shuffle (purpose: demonstrate how to transform an entire deck)
	std::reverse(deck.begin(), deck.end());
	std::cout << "din      ; ";
	return;
}
void CutNCards(std::vector<int>& deck, int n)
{
	// support funtion to apply a CUT(n) shuffle (purpose: demonstrate how to transform an entire deck)
	if (n > 0)
	{
		std::cout << "cut " << std::setw(5) << n << "; ";
		auto it = deck.begin();
		for (int i = 0; i < n; i++)
		{
			deck.push_back(*it);
			it = deck.erase(it);
		}
	}
	else if (n < 0)
	{
		std::cout << "cut " << std::setw(5) << n << "; ";
		//CutNCards(deck, deck.size() + n);
		auto it = deck.begin();
		auto end = deck.size() + n;
		for (size_t i = 0; i < end; i++)
		{
			deck.push_back(*it);
			it = deck.erase(it);
		}
	}
	return;
}
void DealWithIncrement(std::vector<int>& deck, int n)
{
	// support funtion to apply a DWI(n) shuffle (purpose: demonstrate how to transform an entire deck)
	int len = (int)deck.size();
	std::vector<int> newVec;  newVec.assign(deck.size(), 0);
	std::cout << "dwi " << std::setw(5) << n << "; ";
	std::vector<int> copy = deck;
	int offset = 0;
	for (size_t i = 0; i < copy.size(); i++)
	{
		if (((i * n) % copy.size() == 0) && i != 0)
		{
			offset++;
		}
		int index = (i * n) % copy.size() + offset;
		newVec[index] = copy[i];
	}
	deck = newVec;
	return;
}
enum class ShuffleType
{
	DIN, // Deal into new (reverse the deck)
	DWI, // Deal with increment (requires integer n)
	CUT  // Cut the deck (requires integer n)
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
			newIndex = (newIndex * m.n) % deckSize;
			break;
		}
	}
	return newIndex;
}
void LoadMoves(std::vector<MOVE>& moves)
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
		if (str.find("cut") != std::string::npos) // instruction: cut
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
#define LL long long
uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
	// Modular multiplication, to avoid overflow with int64 
	long double x;
	uint64_t c;
	int64_t r;
	if (a >= m) a %= m;
	if (b >= m) b %= m;
	x = (long double)a;
	c = (uint64_t)x * b / m;
	r = (int64_t)(a * b - c * m) % (int64_t)m;
	return r < 0 ? r + m : r;
}
LL binpow_recursive(LL a, LL b)
{
	if (b == 0)
		return 1;
	LL res = binpow_recursive(a, b / 2);
	if (b % 2)
		return res * res * a;
	else
		return res * res;
}
LL binpow_recursive_undermod(LL a, LL b, LL mod)
{
	if (b == 0)
		return 1;
	LL res = binpow_recursive_undermod(a, b / 2, mod);
	if (b % 2)
		return mul_mod(mul_mod(res, res, mod), a, mod); // (res * res * a) with modular multiplication
	else
		return mul_mod(res, res, mod); // (res * res)
}
LL binpow(LL a, LL b)
{
	LL res = 1;
	while (b > 0) {
		if (b & 1)
			res = res * a;
		a = a * a;
		b >>= 1;
	}
	return res;
}
LL binpow_undermod(LL a, LL b, LL mod)
{
	// Binary exponentiation under modulo to calculate: a^b mod m
	// Note a = base, b = exponent, m = modulo
	a %= mod;
	LL res = 1;
	while (b > 0) {
		if (b & 1)
			res = mul_mod(res, a, mod);//res = res * a % mod;
		a = mul_mod(a, a, mod);//a = a * a % mod;
		b >>= 1;
	}
	return res;
}
LL extended_euclidean(LL a, LL b, LL& x, LL& y)
{
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	LL x1, y1;
	LL d = extended_euclidean(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}
static inline LL mod(LL a, LL b)
{
	return (a >= 0) ? (a % b) : (b + a % b);
}
LL modular_power(LL base, LL exponent, LL n)
{
	assert(exponent >= 0);
	if (exponent == 0) {
		return (base == 0) ? 0 : 1;
	}

	LL bit = 1;
	LL power = mod(base, n);
	LL out = 1;
	while (bit <= exponent) {
		if (exponent & bit) {
			out = mod(mul_mod(out, power, n), n);
		}
		power = mod(mul_mod(power, power, n), n);
		bit <<= 1;
	}

	return out;
}
LL ApplyMoves(LL index, LL N, std::vector<MOVE> moves)
{
	LL i = index;
	for (auto m : moves)
	{
		switch (m.sType)
		{
		case ShuffleType::DIN:
			i = (N - i - 1);
			break;
		case ShuffleType::CUT:
			i = (N - m.n + i) % N;
			break;
		case ShuffleType::DWI:
			i = (m.n * i) % N;
			break;
		}
	}
	return (i + N) % N;
}
LL ApplyReverseMoves(LL index, LL N, std::vector<MOVE> moves)
{
	LL i = index;
	for (auto m : moves)
	{
		switch (m.sType)
		{
		case ShuffleType::DIN:
			i = (N - i - 1);
			break;
		case ShuffleType::CUT:
			i = (N + m.n + i) % N;
			break;
		case ShuffleType::DWI:
			i = mul_mod(binpow_undermod(m.n, N - 2, N), i, N) % N;
			break;
		}
	}
	return (i + N) % N;
}
LL MMI(LL n, LL mod)
{
	// Uses binary exponentiation under modulo 
	// Only works if mod is prime (otherwise use extended_euclidean())
	return binpow_undermod(n, mod - 2, mod);
}
