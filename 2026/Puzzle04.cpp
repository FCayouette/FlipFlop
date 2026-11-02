import std;

enum class Flower : char
{
	base,
	stem,
	left,
	right,
	flower
};

int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock().now();
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle04.exe inputFilename\n";
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
	std::vector<Flower> flower;
	while (in >> line)
	{
		if (flower.size() < 3)
			flower.push_back(Flower::flower);
		else
		{
			if (line[0] == '#')
				flower.push_back(Flower::base);
			else if (line.contains("o-"))
				flower.push_back(Flower::left);
			else if (line.contains("-o"))
				flower.push_back(Flower::right);
			else flower.push_back(Flower::stem);
		}
	}
	std::reverse(flower.begin(), flower.end());
	flower.resize(flower.size() - 3);
	result1 = std::count_if(flower.cbegin() + (flower.size() < 400 ? 9 : 401), flower.cend(), [](Flower f) {return f == Flower::left || f == Flower::right; });
	
	flower.erase(std::remove(flower.begin(), flower.end(), Flower::stem), flower.end());
	Flower last = Flower::stem;
	for (Flower f : flower)
	{
		if (f == Flower::left)
		{
			if (last == Flower::right)
				++result2;
			last = f;
		}
		else if (f == Flower::right)
		{
			if (last == Flower::left)
				++result2;
			last = f;
		}
	}

	while (true)
	{
		last = Flower::base;
		int lastIndex = 0, index = 0;
		
		for (Flower f : flower)
		{
			if (f == Flower::left)
			{
				if (last == Flower::right)
					flower[lastIndex] = Flower::stem;
				last = f;
				lastIndex = index;
			}
			else if (f == Flower::right)
			{
				if (last == Flower::left)
					flower[lastIndex] = Flower::stem;
				last = f;
				lastIndex = index;
			}
			++index;
		}

		if (last == Flower::base)
			break;
		++result3;
		flower[lastIndex] = Flower::stem;
		flower.erase(std::remove(flower.begin(), flower.end(), Flower::stem), flower.end());
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock().now() - start));
	return 0;
}