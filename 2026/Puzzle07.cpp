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
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	constexpr bool operator!=(const Coord& p) const { return !IsEqual(x, p.x) || !IsEqual(y, p.y); }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };
constexpr std::array<char, 4> coords = { '>', '^', '<', 'v' };

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

	std::string moves;
	in >> moves;
	int x, y;
	char comma;
	std::vector<Point> sushis;
	while (in >> x >> comma >> y)
		sushis.emplace_back(x, y);

	std::deque<Point> snake = { Point{0, 0} };
	size_t sushiIndex = 0, result1 = 0, result2 = 0, result3 = 0;
	for (size_t i = 0; i < moves.size(); ++i)
	{
		if (i == moves.size() / 2)
			result1 = sushiIndex;

		snake.emplace_back(snake.back() + directions[std::distance(coords.cbegin(), std::find(coords.cbegin(), coords.cend(), moves[i]))]);
		if (sushiIndex == sushis.size() || snake.back() != sushis[sushiIndex])
			snake.pop_front();
		else 
			++sushiIndex;
		if (auto iter = std::find(snake.cbegin(), snake.cend()-1, snake.back()); iter != snake.cend()-1)
		{
			if (!result2)
				result2 = snake.size();
			snake.erase(snake.cbegin(), iter+2); // +2 as eating is considered before moving the entire snake
			++result3;
		}
	}
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3*snake.size());
	return 0;
}