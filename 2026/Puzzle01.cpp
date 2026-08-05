import std;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle01.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	int result1 = 0, result2 = 0, value;
	std::vector<int> temperatures;
	while (in >> value)
	{
		result1 += std::max(60 - value, 0);
		result2 += std::max(value - 60, 0) * 5;
		temperatures.push_back(value);
	}
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2 + result1, 
		std::inner_product(temperatures.cbegin() + temperatures.size() / 2, temperatures.cend(), temperatures.cbegin(), 0, std::plus(),
		[](int preferred, int current) {int diff = current - preferred; return diff < 0 ? -diff : 5 * diff; }));
	return 0;
}
