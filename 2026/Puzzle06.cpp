import std;

template <typename T>
constexpr bool IsEqual(T a, T b)
{
	if constexpr (std::is_floating_point_v<T>)
		return (a < b ? b - a : a - b) < static_cast<T>(0.000001);
	else
		return a == b;
}

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };
constexpr Point INVALID{ -2,-2 };

template<typename T>
struct CoordHasher
{
	constexpr size_t operator()(const Coord<T>& c) const { return 256 * (c.x & 0xff) | (c.y & 0xff); }
};

constexpr std::array<size_t, 10> primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

constexpr bool IsPrime(size_t i)
{
	if (i <= primes.back()) return std::binary_search(primes.cbegin(), primes.cend(), i);
	for (size_t p : primes)
	{
		if (i % p == 0) return false;
		if (p * p > i) return true;
	}
	return true;
}

using u64 = unsigned long long;

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

	std::string line;
	std::vector<Point> lights, gears, extraGears;
	std::unordered_set<Point, CoordHasher<int>> ccw, cw;
	std::vector<std::pair<Point, Point>> bluetooth, backup; // pair data structure:first -> input, second -> output
	int lines = 0;
	bool oddIsCCW;
	while (in >> line)
	{
		for (int i = 0; i < line.size(); ++i)
		{
			if (char c = line[i]; c == 'S')
			{
				Point p{ lines, i };
				oddIsCCW = (p.x + p.y) % 2 != 0;
				gears.emplace_back(std::move(p));
			}
			else if (c == '*')
				lights.emplace_back(lines, i);
			else if (c == '#')
				gears.emplace_back(lines, i);
			else if (c == '3')
				extraGears.emplace_back(lines, i);
			else if (c >= 'a' && c <= 'z')
			{
				size_t index = c - 'a';
				if (index >= bluetooth.size())
					bluetooth.resize(index + 1, { INVALID, INVALID });
				bluetooth[index].first = Point(lines, i);
			}
			else if (c >= 'A' && c <= 'Z')
			{
				size_t index = c - 'A';
				if (index >= bluetooth.size())
					bluetooth.resize(index + 1, { INVALID, INVALID });
				bluetooth[c - 'A'].second = Point(lines, i);
			}
		}
		++lines;
	}

	for (Point p : gears)
	{
		if (((p.x + p.y) % 2 != 0) == oddIsCCW)
			ccw.insert(p);
		else 
			cw.insert(p);
	}

	auto GetResult = [&lights, &cw, &ccw]() -> u64
		{
			u64 result = 0;
			for (Point l : lights)
			{
				bool found = false;
				for (Point d : directions)
					if (Point p = l + d; cw.contains(p))
					{
						result = result * 2 + 1;
						found = true;
						break;
					}
				if (!found)
					for (Point d : directions)
						if (Point p = l + d; ccw.contains(p))
						{
							result <<= 1;
							break;
						}
			}
			return result;
		};
	u64 result1 = GetResult();
	std::unordered_set<Point, CoordHasher<int>> partialCW = cw, partialCCW = ccw;
	backup = bluetooth;

	auto GetGroup = [&](Point s) -> std::unordered_set<Point, CoordHasher<int>>
		{
			std::unordered_set<Point, CoordHasher<int>> processed{ s }, cur = processed, next;
			while (!cur.empty())
			{
				for (Point p : cur)
					for (Point d : directions)
						if (Point np = p + d; std::binary_search(extraGears.cbegin(), extraGears.cend(), np) && processed.insert(np).second)
							next.insert(np);
				std::swap(cur, next);
				next.clear();
			}
			return processed;
		};

	auto ProcessBluetooth = [&](bool primeCheck = false) -> void
		{
			bool nextToCW = false;
			int processPasses = 0;
			while (processPasses < 2 && !bluetooth.empty())
			{
				++processPasses;
				for (auto& [s, e] : bluetooth)
					for (Point d : directions)
						if (Point np = s + d; nextToCW ? cw.contains(np) : ccw.contains(np))
						{
							for (Point d2 : directions)
								if (Point ne = e + d2; std::binary_search(extraGears.cbegin(), extraGears.cend(), ne))
									if (auto group = GetGroup(ne); !primeCheck || (primeCheck && !IsPrime(group.size())))
									{
										processPasses = 0;
										bool oddIsCCW = ((e.x + e.y) % 2 != 0) ^ nextToCW;
										for (Point p : group)
											if (((p.x + p.y) % 2 != 0) == oddIsCCW) ccw.insert(p);
											else cw.insert(p);
									}
							s = INVALID;
							break;
						}
				nextToCW = !nextToCW;
				if (!processPasses)
					bluetooth.erase(std::remove_if(bluetooth.begin(), bluetooth.end(), [](const auto& pair) {return pair.first == INVALID; }), bluetooth.end());
			}
		};
	ProcessBluetooth(false);
	u64 result2 = GetResult();

	std::swap(bluetooth, backup);
	cw = partialCW;
	ccw = partialCCW;

	ProcessBluetooth(true);

	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, GetResult());
	return 0;
}