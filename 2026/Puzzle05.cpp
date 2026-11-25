import std;

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	auto& GetAt(U& grid) const { return grid[x][y]; }
	template<typename U, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	T x = {}, y = {};
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: Puzzle05.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open {}\n", argv[1]);
		return 1;
	}

	std::vector<std::string> map;
	std::string line;
	while (in >> line)
		map.emplace_back(std::move(line));

	std::map<char, Point> directions = { {'>', Point{0,1}}, {'<', Point{0,-1}}, {'^', Point{-1,0}}, {'v', Point{1,0}} };

	constexpr std::array<char, 4> changes = { '<', '^', '>', 'v' };
	auto ComputePathLength = [&map, &directions, &changes](std::set<Point>& path, int maxTurns)
		{
			path.clear();
			Point cur = { 0, 0 };
			int illegalTurns = 0;
			while (illegalTurns <= maxTurns)
			{
				char c = cur.GetAt(map);
				if (path.insert(cur).second)
					cur += directions[c];
				else
				{
					if (cur.x == 0 || cur.y == 0 || cur.x == map.size() - 1 || cur.y == map[0].size() - 1)
						break;
					++illegalTurns;
					size_t i = std::distance(changes.cbegin(), std::find(changes.cbegin(), changes.cend(), c));
					cur += directions[changes[(i + 1) % 4]];
				}
			}
		};
	auto BaseAndBest = [&map, &directions, &ComputePathLength](int maxTurns) -> std::pair<size_t, size_t>
		{
			std::set<Point> base, work;
			ComputePathLength(base, maxTurns);
			size_t best = 0;
			for (Point p : base)
				if (p.x && p.y && p.x != map.size() - 1 && p.y != map[0].size() - 1)
				{
					char original = p.GetAt(map);
					for (char c : changes)
						if (c != original)
						{
							p.GetAt(map) = c;
							ComputePathLength(work, maxTurns);
							best = std::max(best,work.size());
						}
					p.GetAt(map) = original;
				}
			return { base.size(), best };
		};
	auto [result1, result2] = BaseAndBest(0);
	auto [_, result3] = BaseAndBest(3);

	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}