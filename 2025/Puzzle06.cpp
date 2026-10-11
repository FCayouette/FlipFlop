import std;

template<typename T>
struct Coord
{
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord operator * (T scalar) const { return Coord(x * scalar, y * scalar); }
	constexpr Coord& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
	constexpr Coord operator % (const Coord& p) const { return Coord(x % p.x, y % p.y); }
	
	T x = {}, y = {};
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle06.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	constexpr Point bounds = { 1000,1000 }, unitBounds = { 10,10 };
	int result1 = 0, result2 = 0, result3 = 0;
	int x, y;
	char c;
	std::set<std::pair<int, int>> pairs;
	while (in >> x >> c >> y)
	{
		if (!x || !y) continue;
		if (x < 0)
			x += 1000;
		if (y < 0)
			y += 1000;
		Point vel((x * 926) % 1000, (y * 926) % 1000), pos{}, unit(x % 10, y % 10), unitPos = pos;
		if (unit.x >= 3 && unit.x <= 7 && unit.y >= 3 && unit.y <= 7)
			++result1;
		unit = unit * 6 % unitBounds;
		bool found = false;
		for (int i = 0; i < 1000; ++i)
		{
			if (!found)
			{
				unitPos = (unitPos + unit) % unitBounds;
				if (unitPos.x >= 3 && unitPos.x <= 7 && unitPos.y >= 3 && unitPos.y <= 7)
				{
					result2 += 400;
					pairs.insert({ x % 10, y % 10 });
					found = true;
				}
			}	

			pos = (pos + vel) % bounds;
			if (pos.x >= 250 && pos.y >= 250 && pos.x < 750 && pos.y < 750)
				++result3;
		}
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}