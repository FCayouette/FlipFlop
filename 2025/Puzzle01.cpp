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

	int result1 = 0, result2 = 0, result3 = 0;
	std::string line;
	while (in >> line)
	{
		int score = line.size() / 2;
		result1 += score;
		if (score % 2 == 0)
			result2 += score;
		if (line.find('e') == std::string::npos)
			result3 += score;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}
