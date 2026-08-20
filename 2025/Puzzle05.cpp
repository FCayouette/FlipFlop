import std;

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

	int result1 = 0, result3 = 0;
	std::string line, result2;
	in >> line;
	std::map<char, std::pair<size_t, size_t>> tunnels;
	std::set<char> unused;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (auto iter = tunnels.find(line[i]); iter != tunnels.end())
			iter->second.second = i;
		else
		{
			tunnels[line[i]] = std::make_pair(i, i);
			unused.insert(line[i]);
		}
	}
	
	size_t pos = 0;
	while (pos < line.size())
	{
		const std::pair<size_t, size_t>& tunnel = tunnels[line[pos]];
		if (tunnel.first == pos)
			pos = tunnel.second;
		else
			pos = tunnel.first;
		if (auto iter = unused.find(line[pos]); iter != unused.cend())
			unused.erase(iter);
		int delta = tunnel.second - tunnel.first;
		result1 += delta;
		result3 += (line[pos] >= 'A' && line[pos] <= 'Z') ? -delta : delta;
		++pos;
	}
	for (char c : line)
		if (auto iter = unused.find(c); iter != unused.cend())
		{
			unused.erase(iter);
			result2 += c;
		}


	std::cout << std::format("Part 1: {}\nPart 2: {}\nPart 3: {}\n", result1, result2, result3);
	return 0;
}