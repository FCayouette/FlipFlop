import std;

using u64 = unsigned long long;

constexpr u64 factorial(int f)
{
	u64 result = 1ull;
	for (int i = 2; i <= f; ++i)
		result *= (u64)i;
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle07.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	u64 result1 = 0, result2 = 0, result3 = 0;
	int x, y;
	while (in >> x >> y)
	{
		u64 xfactor = factorial(x - 1), yfactor = factorial(y-1), topFactor = factorial(x * (y - 1));
		result1 += factorial(x + y - 2) / (xfactor * yfactor);
		result2 += factorial(2 * x + y - 3) / (xfactor * xfactor * yfactor);
		for (int i = 0; i < x; ++i)
			topFactor /= yfactor;
		result3 += topFactor;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}