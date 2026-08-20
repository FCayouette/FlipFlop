import std;

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	T x = {}, y = {};
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
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

	int result1 = 0, result2 = 0, x, y;
	char c;
	Point cur{ 0,0 };
	std::vector<std::pair<int, Point>> trash = { {0, cur} };
	while (in >> x >> c >> y)
	{
		int dx = std::abs(cur.x - x), dy = std::abs(cur.y - y);
		result1 += dx + dy;
		result2 += std::max(dx, dy);
		Point next{ x,y };
		cur = next;
		trash.emplace_back(x + y, std::move(next));
	}
	
	std::sort(trash.begin(), trash.end());
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, 
		std::inner_product(trash.cbegin(), std::prev(trash.cend()), std::next(trash.cbegin()), 0, std::plus<>(),
		[](const std::pair<int, Point>& l, const std::pair<int, Point>& r) { return std::max(std::abs(l.second.x - r.second.x), std::abs(l.second.y - r.second.y)); }));
	return 0;
}