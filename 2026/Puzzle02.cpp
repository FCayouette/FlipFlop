import std;

#define ALLc(x) (x).cbegin(),(x).cend()

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle02.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	int pos = 0, laser = 0, wall = 0, result1 = 0, result2 = 0;
	std::array<int, 100> heat;
	heat.fill(0);
	std::string line;
	in >> line;
	for (char c : line)
	{
		pos = (pos + (c == '<' ? 99 : 1)) % 100;
		++heat[pos];
	}
	auto maxHeat = std::max_element(ALLc(heat));
	result1 = *maxHeat * (int)(std::distance(heat.cbegin(), maxHeat) + 1);
	result2 = std::inner_product(ALLc(line), line.crbegin(), 0, std::plus(),
		[&laser, &wall](char l, char w) {laser = (laser + (l == '<' ? 99 : 1)) % 100;
		wall = (wall + (w == '<' ? 99 : 1)) % 100;
		return (int)(wall == laser); });

	heat.fill(0);
	laser = 0, wall = 0;
	for (size_t i = 0, j = line.size()-1; i < line.size(); ++i, --j)
	{
		laser = (laser + (line[i] == '<' ? 99 : 1)) % 100;
		laser = (laser + (line[j] == '<' ? 1 : 99)) % 100;
		wall = (wall + (line[j] == '<' ? 1 : 99)) % 100;
		++heat[laser];
	}
	std::rotate(heat.begin(), heat.begin() + wall, heat.end());
	maxHeat = std::max_element(ALLc(heat));
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, *maxHeat * ((std::distance(heat.cbegin(), maxHeat) + wall) % 100 + 1));
	return 0;
}