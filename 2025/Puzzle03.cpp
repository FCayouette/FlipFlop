import std;

struct Color
{
	int r = {}, g = {}, b = {};
	Color() = default;
	Color(std::string&& s)
	{
		r = (s[0] - '0') * 10 + s[1] - '0';
		g = (s[3] - '0') * 10 + s[4] - '0';
		b = (s[6] - '0') * 10 + s[7] - '0';
	}
	constexpr bool IsRed() const { return r > g && r > b && g != b;	}
	constexpr bool IsGreen() const { return g > r && g > b && r != b; }
	constexpr bool IsBlue() const {	return b > r && b > g && r != g; }
	constexpr bool operator<(const Color& c) const { if (r < c.r) return true; if (c.r < r) return false; if (g < c.g) return true; if (c.g < g) return false; return b < c.b; }
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle03.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	int result2 = 0, result3 = 0, best = 0;
	std::string line;
	std::map<Color, int> histogram;
	while (in >> line)
		++histogram[Color(std::move(line))];
	
	Color mostCommon;
	for (auto& [c, count] : histogram)
	{
		if (count > best)
		{
			best = count;
			mostCommon = c;
		}
		if (c.IsGreen())
		{
			result2 += count;
			result3 += 2 * count;
		}
		else if (c.IsRed())
			result3 += 5 * count;
		else if (c.IsBlue())
			result3 += 4 * count;
		else
			result3 += 10 * count;
	}
	std::cout << std::format("Part 1: {},{},{}\nPart 2: {}\nPart 3: {}\n", mostCommon.r, mostCommon.g, mostCommon.b, result2, result3);
	return 0;
}