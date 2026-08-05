import std;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()
#define ALLr(x) (x).rbegin(),(x).rend()
#define ALLcr() (x).crbegin(),(x).crend()

using u64 = unsigned long long;
using i64 = long long;

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

	i64 result1 = 0, result2 = 0, result3 = 0;
	std::string line;
	in >> line;
	std::vector<int> fib = { 1,1,2 };
	auto Fibonacci = [&fib](int i)
		{
			if (!i) return 0;
			while (fib.size() < i)
				fib.push_back(*fib.crbegin() + *std::next(fib.crbegin()));
			return fib[i - 1];
		};

	i64 r1height = 0, upMomentum = 0, downMomentum = 0, r2height = 0, r3height = 0;
	for (char c : line)
	{
		if (c == '^')
		{
			result1 = std::max(++r1height, result1);
			r3height -= Fibonacci(downMomentum);
			downMomentum = 0;
			++upMomentum;
			r2height += upMomentum;
			result2 = std::max(result2, r2height);
		}
		else
		{
			--r1height;
			r3height += Fibonacci(upMomentum);
			result3 = std::max(r3height, result3);
			upMomentum = 0;
			++downMomentum;
			r2height -= downMomentum;
		}
	}
	r3height += Fibonacci(upMomentum);
	result3 = std::max(r3height, result3);

	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}