import std;

std::pair<int, int> ComputeStrength(const std::string& password)
{
	bool lower = false, upper = false, digit = false, hasSeven = false, hasOtherDigit = false;
	char previous = '-';
	int seqLength = 1, longestSequence = 1;
	for (char c : password)
	{
		if (c >= 'a' && c <= 'z') lower = true;
		else if (c >= 'A' && c <= 'Z') upper = true;
		else if (c >= '0' && c <= '9')
		{
			digit = true;
			if (c == '7') hasSeven = true;
			else hasOtherDigit = true;
		}
		if (c == previous)
			longestSequence = std::max(++seqLength, longestSequence);
		else
			seqLength = 1;
		previous = c;
	}
	int baseStrength = (lower + upper + digit), strength2 = baseStrength + (longestSequence >= 3 ? longestSequence * longestSequence : 0) + (hasSeven && !hasOtherDigit ? 7 : 0);
	strength2 *= password.size();
	if (password.contains("red") || password.contains("green") || password.contains("blue"))
		strength2 *= 3;
	return { baseStrength * password.size(), strength2 };
}

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

	int result1 = 0, result2 = 0, result3 = 0;
	std::string line, pass1, pass2;
	std::vector<std::string> basePasswords;
	while (in >> line)
	{
		auto [s1, s2] = ComputeStrength(line);
		if (s1 > result1)
		{
			result1 = s1;
			pass1 = line;
		}
		if (s2 > result2)
		{
			result2 = s2;
			pass2 = line;
		}
		basePasswords.emplace_back(std::move(line));
	}
	auto TestRangeInclusive = [&result3, &basePasswords](char a, char b)
		{
			for (char c = a; c <= b; ++c)
				result3 = std::max(result3, std::reduce(basePasswords.cbegin(), basePasswords.cend(), 0, [c](int t, const std::string& s) { return t + ComputeStrength(s + c).second; }));
		};
	TestRangeInclusive('0', '9');
	TestRangeInclusive('a', 'z');
	TestRangeInclusive('A', 'Z');

	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", pass1, pass2, result3);
	return 0;
}