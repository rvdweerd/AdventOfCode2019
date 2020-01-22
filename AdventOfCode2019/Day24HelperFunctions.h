#pragma once

void PrintEris(const std::string& field, int fieldWidth)
{
	for (size_t i = 0; i < field.size(); i++)
	{
		if (i % fieldWidth == 0 && i != 0) std::cout << '\n';
		std::cout << field[i];
	}
	std::cout << '\n';
}
void PrintEris(const unsigned int field, int fieldWidth)
{
	std::cout << '\n';
	for (int i = 0; i < 25; i++)
	{
		if (i % fieldWidth == 0 && i != 0) std::cout << '\n';
		unsigned int bitMask = 0b1 << i;
		std::cout << (bitMask & field ? '#' : '.');
	}
	std::cout << '\n';
}
unsigned int HashToUInt(const std::string& field)
{
	unsigned int res = 0b0;
	unsigned int mash = 0b1;
	for (size_t i = 0; i < field.size(); i++)
	{
		if (field[i] == '#')
		{
			// Set ith bit to 1
			res = res | mash;
		}
		mash <<= 1;
	}
	return res;
}
std::string ToBin(unsigned int n, int min_digits = 0)
{
	std::string bin_str;
	for (int count = 0; n != 0 || count < min_digits; n >>= 1, count++)
	{
		bin_str.push_back(bool(n & 0b1) ? '1' : '0');
	}
	std::reverse(bin_str.begin(), bin_str.end());
	return bin_str;
}
bool bugAt(int x, int y, const std::string& field, const int fieldWidth)
{
	return int(field[y * fieldWidth + x] == '#');
}
bool bugAt(int x, int y, const unsigned int field, const int fieldWidth)
{
	unsigned int bitMask = 0b1 << (y * fieldWidth + x);
	return (bitMask & field);
}
int countAdjacentBugs(int x, int y, const std::string& field, const int fieldWidth)
{
	int fieldHeight = field.size() / fieldWidth;
	int count = 0;
	if (y != 0)				count += bugAt(x, y - 1, field, fieldWidth); // CHECK NORTH
	if (y != fieldHeight - 1) count += bugAt(x, y + 1, field, fieldWidth); // CHECK SOUTH
	if (x != 0)				count += bugAt(x - 1, y, field, fieldWidth); // CHECK WEST
	if (x != fieldWidth - 1)	count += bugAt(x + 1, y, field, fieldWidth); // CHECK EAST
	return count;
}
int countAdjacentBugs(int x, int y, const unsigned int field, const int fieldWidth)
{
	int fieldHeight = fieldWidth;
	int count = 0;
	if (y != 0)					count += bugAt(x, y - 1, field, fieldWidth); // CHECK NORTH
	if (y != fieldHeight - 1)	count += bugAt(x, y + 1, field, fieldWidth); // CHECK SOUTH
	if (x != 0)					count += bugAt(x - 1, y, field, fieldWidth); // CHECK WEST
	if (x != fieldWidth - 1)	count += bugAt(x + 1, y, field, fieldWidth); // CHECK EAST
	return count;
}
int countAdjacentBugs(int x, int y, const unsigned int field0, const unsigned int fieldMin1, const unsigned int fieldPlus1, const int fieldWidth)
{
	if (field0 == 0 && fieldMin1 == 0 && fieldPlus1 == 0) return 0;
	int fieldHeight = fieldWidth;
	int count = 0;
	if (y == 0 && x == 0)
	{
		count = count + bugAt(1, 0, field0, fieldWidth) + bugAt(0, 1, field0, fieldWidth) + bugAt(2, 1, fieldMin1, fieldWidth) + bugAt(1, 2, fieldMin1, fieldWidth);
	}
	else if (y == 0 && x == 4)
	{
		count = count + bugAt(3, 0, field0, fieldWidth) + bugAt(4, 1, field0, fieldWidth) + bugAt(2, 1, fieldMin1, fieldWidth) + bugAt(3, 2, fieldMin1, fieldWidth);
	}
	else if (y == 0)
	{
		count = count + bugAt(x, 1, field0, fieldWidth) + bugAt(x - 1, 0, field0, fieldWidth) + bugAt(x + 1, 0, field0, fieldWidth) + bugAt(2, 1, fieldMin1, fieldWidth);
	}

	else if (y == 1 && x == 0)
	{
		count = count + bugAt(0, 0, field0, fieldWidth) + bugAt(1, 1, field0, fieldWidth) + bugAt(0, 2, field0, fieldWidth) + bugAt(1, 2, fieldMin1, fieldWidth);
	}
	else if (y == 1 && x == 2)
	{
		count = count + bugAt(2, 0, field0, fieldWidth) + bugAt(1, 1, field0, fieldWidth) + bugAt(3, 1, field0, fieldWidth) +
			bugAt(0, 0, fieldPlus1, fieldWidth) + bugAt(1, 0, fieldPlus1, fieldWidth) + bugAt(2, 0, fieldPlus1, fieldWidth) +
			bugAt(3, 0, fieldPlus1, fieldWidth) + bugAt(4, 0, fieldPlus1, fieldWidth);
	}
	else if (y == 1 && x == 4)
	{
		count = count + bugAt(3, 1, field0, fieldWidth) + bugAt(4, 0, field0, fieldWidth) + bugAt(4, 2, field0, fieldWidth) + bugAt(3, 2, fieldMin1, fieldWidth);
	}

	else if (y == 2 && x == 0)
	{
		count = count + bugAt(0, 1, field0, fieldWidth) + bugAt(0, 3, field0, fieldWidth) + bugAt(1, 2, field0, fieldWidth) + bugAt(1, 2, fieldMin1, fieldWidth);
	}
	else if (y == 2 && x == 1)
	{
		count = count + bugAt(1, 1, field0, fieldWidth) + bugAt(0, 2, field0, fieldWidth) + bugAt(1, 3, field0, fieldWidth) +
			bugAt(0, 0, fieldPlus1, fieldWidth) + bugAt(0, 1, fieldPlus1, fieldWidth) + bugAt(0, 2, fieldPlus1, fieldWidth) +
			bugAt(0, 3, fieldPlus1, fieldWidth) + bugAt(0, 4, fieldPlus1, fieldWidth);
	}
	else if (y == 2 && x == 3)
	{
		count = count + bugAt(3, 1, field0, fieldWidth) + bugAt(4, 2, field0, fieldWidth) + bugAt(3, 3, field0, fieldWidth) +
			bugAt(4, 0, fieldPlus1, fieldWidth) + bugAt(4, 1, fieldPlus1, fieldWidth) + bugAt(4, 2, fieldPlus1, fieldWidth) +
			bugAt(4, 3, fieldPlus1, fieldWidth) + bugAt(4, 4, fieldPlus1, fieldWidth);
	}
	else if (y == 2 && x == 4)
	{
		count = count + bugAt(4, 1, field0, fieldWidth) + bugAt(4, 3, field0, fieldWidth) + bugAt(3, 2, field0, fieldWidth) + bugAt(3, 2, fieldMin1, fieldWidth);
	}

	else if (y == 3 && x == 0)
	{
		count = count + bugAt(0, 2, field0, fieldWidth) + bugAt(0, 4, field0, fieldWidth) + bugAt(1, 3, field0, fieldWidth) + bugAt(1, 2, fieldMin1, fieldWidth);
	}
	else if (y == 3 && x == 2)
	{
		count = count + bugAt(1, 3, field0, fieldWidth) + bugAt(3, 3, field0, fieldWidth) + bugAt(2, 4, field0, fieldWidth) +
			bugAt(0, 4, fieldPlus1, fieldWidth) + bugAt(1, 4, fieldPlus1, fieldWidth) + bugAt(2, 4, fieldPlus1, fieldWidth) +
			bugAt(3, 4, fieldPlus1, fieldWidth) + bugAt(4, 4, fieldPlus1, fieldWidth);
	}
	else if (y == 3 && x == 4)
	{
		count = count + bugAt(3, 3, field0, fieldWidth) + bugAt(4, 2, field0, fieldWidth) + bugAt(4, 4, field0, fieldWidth) + bugAt(3, 2, fieldMin1, fieldWidth);
	}

	else if (y == 4 && x == 0)
	{
		count = count + bugAt(0, 3, field0, fieldWidth) + bugAt(1, 4, field0, fieldWidth) + bugAt(1, 2, fieldMin1, fieldWidth) + bugAt(2, 3, fieldMin1, fieldWidth);
	}
	else if (y == 4 && x == 4)
	{
		count = count + bugAt(4, 3, field0, fieldWidth) + bugAt(3, 4, field0, fieldWidth) + bugAt(3, 2, fieldMin1, fieldWidth) + bugAt(2, 3, fieldMin1, fieldWidth);
	}
	else if (y == 4)
	{
		count = count + bugAt(x, 3, field0, fieldWidth) + bugAt(x - 1, 4, field0, fieldWidth) + bugAt(x + 1, 4, field0, fieldWidth) + bugAt(2, 3, fieldMin1, fieldWidth);
	}

	else
	{
		count = count + bugAt(x - 1, y, field0, fieldWidth) + bugAt(x + 1, y, field0, fieldWidth) + bugAt(x, y - 1, field0, fieldWidth) + bugAt(x, y + 1, field0, fieldWidth);
	}
	return count;
}
void ApplyTimeStep(std::string& field, const int fieldWidth)
{
	std::string field_copy = field;
	for (size_t y = 0; y < field.size() / fieldWidth; y++)
	{
		for (int x = 0; x < fieldWidth; x++)
		{
			if (bugAt(x, y, field, fieldWidth))
			{
				if (countAdjacentBugs(x, y, field, fieldWidth) != 1) field_copy[y * fieldWidth + x] = '.';
			}
			else
			{
				int n = countAdjacentBugs(x, y, field, fieldWidth);
				if (n == 1 || n == 2) field_copy[y * fieldWidth + x] = '#';
			}
		}
	}
	field = field_copy;
	return;
}
void ApplyTimeStep(unsigned int& field, const int fieldWidth)
{
	int fieldHeight = fieldWidth;
	unsigned int field_copy = field;
	for (int y = 0; y < fieldHeight; y++)
	{
		for (int x = 0; x < fieldWidth; x++)
		{
			if (bugAt(x, y, field, fieldWidth))
			{
				if (countAdjacentBugs(x, y, field, fieldWidth) != 1) field_copy ^= (0b1 << (y * fieldWidth + x));
			}
			else
			{
				int n = countAdjacentBugs(x, y, field, fieldWidth);
				if (n == 1 || n == 2) field_copy ^= (0b1 << (y * fieldWidth + x));
			}
		}
	}
	field = field_copy;
	return;
}
void ApplyTimeStep(std::map<int, unsigned int>& map, const int fieldWidth)
{
	int mapSize = map.size();
	int newN = mapSize / 2 + 1;
	int fieldHeight = fieldWidth;
	map[-newN] = 0b0;
	map[newN] = 0b0;
	std::map<int, unsigned int> map_copy = map;

	for (int depth = -newN; depth <= newN; depth++)
	{
		for (int y = 0; y < fieldHeight; y++)
		{
			for (int x = 0; x < fieldWidth; x++)

				if (!(x == 2 && y == 2))
				{
					{
						if (bugAt(x, y, map[depth], fieldWidth))
						{
							if (countAdjacentBugs(x, y, map[depth], map[depth - 1], map[depth + 1], fieldWidth) != 1) map_copy[depth] ^= (0b1 << (y * fieldWidth + x));
						}
						else
						{
							int n = countAdjacentBugs(x, y, map[depth], map[depth - 1], map[depth + 1], fieldWidth);
							if (n == 1 || n == 2) map_copy[depth] ^= (0b1 << (y * fieldWidth + x));
						}
					}
				}
		}
	}
	map = map_copy;
	return;
}
unsigned int countSetBits(unsigned int n)
{
	unsigned int count = 0;
	while (n) {
		count += n & 1;
		n >>= 1;
	}
	return count;
}
